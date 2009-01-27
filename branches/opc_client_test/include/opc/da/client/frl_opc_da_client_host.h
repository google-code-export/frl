#ifndef frl_opc_da_client_host_h_
#define frl_opc_da_client_host_h_
#include "frl_platform.h"
#if( FRL_PLATFORM ==FRL_PLATFORM_WIN32 )
#include <map>
#include "opc/da/client/frl_opc_da_client_server_connection.h"

namespace frl{ namespace opc{ namespace da{ namespace client{

class Host : private boost::noncopyable
{
private:
	String name;
	typedef std::pair< String, ServerConnectionPtr > ConnectionListElem;
	typedef std::map< String, ServerConnectionPtr > ConnectionList;
	ConnectionList connection_list;
public:
	FRL_EXCEPTION_CLASS( ConnectionAlreadyAdded );

	Host( const String &name_ );
	String getName();
	ServerConnectionPtr addConnection( const String& server_id );
}; // class Host

typedef boost::shared_ptr< Host > HostPtr;

} // namespace client
} // namespace da
} // namespace opc
} // namespace frl

#endif // FRL_PLATFORM_WIN32
#endif // frl_opc_da_client_host_h_
