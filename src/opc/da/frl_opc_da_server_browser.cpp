#include "opc/da/frl_opc_da_server_browser.h"
#include "../dependency/vendors/opc_foundation/opcda.h"
#include "frl_smart_ptr.h"
#include "frl_exception.h"

namespace frl{ namespace opc{ namespace da{

void ServerBrowser::getServerList( const CATID &interface_, std::vector<String> &to_list )
{
	FRL_EXCEPT_GUARD();
	frl::ComPtr< ICatInformation > cat_info;
	HRESULT result = CoCreateInstance( CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, IID_ICatInformation, (void **)&cat_info );
	if (FAILED(result))
	{
		FRL_THROW( FRL_STR("no call function CoInitialize( NULL )") );
	}

	frl::ComPtr<IEnumCLSID> enum_clsid;
	CATID cat_list[1];
	cat_list[0] = interface_;
	result = cat_info->EnumClassesOfCategories( 1, cat_list, 0, NULL, (IEnumCLSID**)&enum_clsid );

	GUID glist;
	ULONG actual;
	while((result = enum_clsid->Next(1, &glist, &actual)) == S_OK)
	{
		WCHAR *progID;
		HRESULT res = ProgIDFromCLSID(glist, &progID);
		if(FAILED(res))
		{
			FRL_THROW( FRL_STR("not found class") );
		}
		else 
		{
			to_list.push_back( progID );
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

} // namespace da
} // namespace opc
} // namespace FatRat Library
