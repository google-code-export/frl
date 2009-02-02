#ifndef frl_opc_da_client_group_info_h_
#define frl_opc_da_client_group_info_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )

#include <boost/noncopyable.hpp>
#include "../dependency/vendors/opc_foundation/opcda.h"
#include "frl_types.h"
#include "frl_smart_ptr.h"

namespace frl{ namespace opc{ namespace da{ namespace client{

struct GroupInfo : private boost::noncopyable
{
	const String name; // group name
	ComPtr<IOPCServer> server_ptr; // pointer to server object
	OPCHANDLE group_handle; // handle given by the server
	ComPtr<IOPCGroupStateMgt> group_state_mgt; //  pointer to group state management object
	DWORD update_rate; // group update rate
	ComPtr<IOPCItemMgt> item_mgt; // pointer to item management object
	
	GroupInfo( const String& name_, ComPtr<IOPCServer> server_ptr_ )
		:	name( name_ ), server_ptr( server_ptr_ )
	{
	}

};

} // namespace client
} // namespace da
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
#endif // frl_opc_da_client_group_info_h_
