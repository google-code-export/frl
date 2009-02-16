#include "opc/da/client/frl_opc_da_client_server_browser.h"
#if( FRL_PLATFORM ==FRL_PLATFORM_WIN32 )
#include "../dependency/vendors/opc_foundation/opcda.h"
#include "frl_smart_ptr.h"
#include "os/win32/frl_os_win32_exception.h"
#include "frl_lexical_cast.h"
#include "os/win32/registry/frl_registry_Key.h"

namespace frl{ namespace opc{ namespace da{ namespace client {

std::vector<ServerInfo> ServerBrowser::getLocalRegistryAvailableServers()
{
	static size_t max_key_size = 256;
	std::vector< Char > szKey( max_key_size );

	HKEY hk = HKEY_CLASSES_ROOT;
	HKEY hProgID = NULL;
	std::vector<Char> szDummy( 256 );
	LONG param_size;
	DWORD index = 0;
	std::vector<ServerInfo> to_list;
	while( ::RegEnumKey(hk, index, &szKey[0], max_key_size) == ERROR_SUCCESS )
	{
		if(::RegOpenKey(hk, &szKey[0], &hProgID) == ERROR_SUCCESS)
		{
			param_size = max_key_size;
			if(::RegQueryValue(hProgID, FRL_STR("OPC"), &szDummy[0], &param_size) == ERROR_SUCCESS)
			{
				ServerInfo tmp;
				tmp.prog_id = &szKey[0];
				param_size = max_key_size;
				if(::RegQueryValue(hProgID, FRL_STR("CLSID"), &szDummy[0], &param_size) == ERROR_SUCCESS)
				{
					os::win32::registry::Key tmp_key( tmp.prog_id + FRL_STR("\\CLSID"), os::win32::registry::RootKeys::classesRoot );
					tmp.clsid_str = tmp_key.getStringValue();
				}
				to_list.push_back( tmp );
			}
			::RegCloseKey(hProgID);
		}
		++index;
	}
	return to_list;
}

std::vector<ServerInfo> ServerBrowser::getLocalAvailableServers( const CATID &interface_ )
{
	FRL_EXCEPT_GUARD();
	frl::ComPtr< ICatInformation > cat_info;
	HRESULT result = CoCreateInstance( CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, IID_ICatInformation, (void **)&cat_info );
	if (FAILED(result))
		FRL_THROW_SYSAPI_CODE( result );

	frl::ComPtr<IEnumCLSID> enum_clsid;
	CATID cat_list[1];
	cat_list[0] = interface_;
	result = cat_info->EnumClassesOfCategories( 1, cat_list, 0, NULL, (IEnumCLSID**)&enum_clsid );

	GUID glist;
	ULONG actual;
	std::vector<ServerInfo> to_list;
	while( (result = enum_clsid->Next( 1, &glist, &actual) ) == S_OK)
	{
		ServerInfo tmp;
		WCHAR* prog_id = NULL ;
		HRESULT result = ProgIDFromCLSID( glist, &prog_id );
		if( FAILED( result ) )
			FRL_THROW_SYSAPI_CODE_EX( FRL_STR("Can`t convert GUID to ProgID"), result );
		tmp.prog_id = similarCompatibility( prog_id );
		CoTaskMemFree( prog_id );

		tmp.clsid_str = lexicalCast<GUID, String>( glist );
		to_list.push_back( tmp );
	}
	return to_list;
}

std::vector<ServerInfo> ServerBrowser::getLocalAvailableServersDA1()
{
	FRL_EXCEPT_GUARD();
	return getLocalAvailableServers( CATID_OPCDAServer10 );
}

std::vector<ServerInfo> ServerBrowser::getLocalAvailableServersDA2()
{
	FRL_EXCEPT_GUARD();
	return getLocalAvailableServers( CATID_OPCDAServer20 );
}

std::vector<ServerInfo> ServerBrowser::getLocalAvailableServersDA3()
{
	FRL_EXCEPT_GUARD();
	return getLocalAvailableServers( CATID_OPCDAServer30 );
}

} // namespace client
} // namespace da
} // namespace opc
} // namespace FatRat Library

#endif // FRL_PLATFORM_WIN32
