#ifndef frl_opc_da_client_h_
#define frl_opc_da_client_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include <map>
#include "opc/da/client/frl_opc_da_client_host.h"

namespace frl{ namespace opc{ namespace da{ namespace client {

class Client
{
private:
	typedef std::pair< String, HostPtr > HostListElem;
	typedef std::map< String, HostPtr > HostListMap;
	HostListMap host_list;
public:
	FRL_EXCEPTION_CLASS( HostAlreadyAdded );
	Client();
	~Client();
	HostPtr addHost( const String &to_host );
	ServerConnectionPtr addConnection( const HostPtr& to_host );
}; // class Client

} // namespace client
} // namespace da
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM
#endif // frl_opc_da_client_h_
