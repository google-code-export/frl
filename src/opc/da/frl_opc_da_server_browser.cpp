#include "opc/da/frl_opc_da_server_browser.h"
#include "../dependency/vendors/opc_foundation/opcda.h"
#include "frl_smart_ptr.h"
#include "os/win32/frl_os_win32_exception.h"

namespace frl{ namespace opc{ namespace da{

void ServerBrowser::getAllServerListForce( std::vector<String> &to_list )
{
	static size_t max_key_size = 256;
	std::vector< Char > szKey( max_key_size );

	HKEY hk = HKEY_CLASSES_ROOT;
	HKEY hProgID = NULL;
	std::vector<Char> szDummy( 256 );
	LONG param_size;
	DWORD index = 0;
	while( ::RegEnumKey(hk, index, &szKey[0], max_key_size) == ERROR_SUCCESS )
	{
		if(::RegOpenKey(hk, &szKey[0], &hProgID) == ERROR_SUCCESS)
		{
			param_size = max_key_size;
			if(::RegQueryValue(hProgID, FRL_STR("OPC"), &szDummy[0], &param_size) == ERROR_SUCCESS)
				to_list.push_back( &szKey[0] );
			::RegCloseKey(hProgID);
		}
		++index;
	}
}

void ServerBrowser::getServerList( const CATID &interface_, std::vector<String> &to_list )
{
	FRL_EXCEPT_GUARD();
	frl::ComPtr< ICatInformation > cat_info;
	HRESULT result = CoCreateInstance( CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, IID_ICatInformation, (void **)&cat_info );
	if (FAILED(result))
		FRL_THROW_SYSAPI();

	frl::ComPtr<IEnumCLSID> enum_clsid;
	CATID cat_list[1];
	cat_list[0] = interface_;
	result = cat_info->EnumClassesOfCategories( 1, cat_list, 0, NULL, (IEnumCLSID**)&enum_clsid );

	GUID glist;
	ULONG actual;
	while( (result = enum_clsid->Next( 1, &glist, &actual) ) == S_OK)
	{
		WCHAR *progID;
		HRESULT res = ProgIDFromCLSID( glist, &progID );
		if(FAILED(res))
		{
			FRL_THROW_SYSAPI();
		}
		else 
		{
			#if( FRL_CHARACTER == FRL_CHARACTER_UNICODE )
				to_list.push_back( progID );
			#else
				to_list.push_back( wstring2string( progID ) );
			#endif
		}
	}
}

void ServerBrowser::getServerListDA1( std::vector<String> &to_list )
{
	FRL_EXCEPT_GUARD();
	getServerList( CATID_OPCDAServer10, to_list );
}

void ServerBrowser::getServerListDA2( std::vector<String> &to_list )
{
	FRL_EXCEPT_GUARD();
	getServerList( CATID_OPCDAServer20, to_list );
}

void ServerBrowser::getServerListDA3( std::vector<String> &to_list )
{
	FRL_EXCEPT_GUARD();
	getServerList( CATID_OPCDAServer30, to_list );
}

void ServerBrowser::getAllServerList( std::vector<String> &to_list )
{
	FRL_EXCEPT_GUARD();
	getServerListDA1( to_list );
	getServerListDA2( to_list );
	getServerListDA3( to_list );
}

} // namespace da
} // namespace opc
} // namespace FatRat Library
