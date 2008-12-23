#include "opc/da/frl_opc_da_server_browser.h"
#include "../dependency/vendors/opc_foundation/opcda.h"
#include "frl_smart_ptr.h"
#include "frl_exception.h"
#include <COMCat.h>

namespace frl{ namespace opc{ namespace da{

void ServerBrowser::getServerListDA2( std::vector<frl::String> &to_list )
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
	cat_list[0] = CATID_OPCDAServer20;
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

} // namespace da
} // namespace opc
} // namespace FatRat Library
