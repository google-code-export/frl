#ifndef frl_opc_da_client_async_io_2_group_h_
#define frl_opc_da_client_async_io_2_group_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include"opc/da/client/frl_opc_da_client_group_base.h"

namespace frl{ namespace opc{ namespace da{ namespace client {

class AsyncIO2Group : public GroupBase
{
private:
	OPCHANDLE callback_handle; // callback handle given by the server
	ComPtr<IOPCAsyncIO2> async_io;
public:
	AsyncIO2Group( const String& name, ComPtr<IOPCServer> server_ptr_ )
		: GroupBase( name, server_ptr_ )
	{
	}

};

} // namespace client
} // namespace da
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
#endif // frl_opc_da_client_async_io_2_group_h_
