#include "opc/da/client/frl_opc_da_client_group_base.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )

namespace frl{ namespace opc{ namespace da{ namespace client {

GroupBase::GroupBase( const String& name_, ComPtr<IOPCServer> server_ptr_ )
	:	info( name_, server_ptr_ )
{
}


GroupBase::~GroupBase()
{
}

void GroupBase::create()
{
	HRESULT result = info.server_ptr->AddGroup( info.name.c_str(), 
																	FALSE,
																	1000,
																	0,
																	0,
																	0,
																	LOCALE_SYSTEM_DEFAULT,
																	&info.group_handle,
																	&info.update_rate,
																	IID_IOPCGroupStateMgt,
																	(LPUNKNOWN*)(&info.group_state_mgt) );
	if (FAILED(result))
	{
		throw String( FRL_STR("Failed to Add group") );
	}
}

const String& GroupBase::getName()
{
	return info.name;
}

void GroupBase::remove()
{

}

} // namespace client
} // namespace da
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM
