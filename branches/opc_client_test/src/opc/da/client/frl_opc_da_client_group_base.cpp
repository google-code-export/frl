#include "opc/da/client/frl_opc_da_client_group_base.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include "opc/frl_opc_exception.h"

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
	HRESULT result = info.server_ptr->AddGroup( unicodeCompatibility( info.name ).c_str(), 
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
	if( FAILED(result) )
	{
		FRL_THROW_OPC( result );
	}
}

const String& GroupBase::getName()
{
	return info.name;
}

void GroupBase::removeGroup( Bool force_ )
{
	FRL_EXCEPT_GUARD();
	HRESULT result = info.server_ptr->RemoveGroup( info.group_handle, BOOL( force_ ) );
	if( FAILED(result) )
	{
		FRL_THROW_OPC( result );
	}
}

void GroupBase::renameTo( const String& new_name )
{
	FRL_EXCEPT_GUARD();
}

} // namespace client
} // namespace da
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM
