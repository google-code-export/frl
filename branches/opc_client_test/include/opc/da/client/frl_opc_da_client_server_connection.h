#ifndef frl_opc_da_client_server_connection_h_
#define frl_opc_da_client_server_connection_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "../dependency/vendors/opc_foundation/opcda.h"
#include "../dependency/vendors/opc_foundation/opccomn.h"
#include "frl_smart_ptr.h"
#include "frl_exception.h"

namespace frl{ namespace opc{ namespace da{ namespace client{

class ServerConnection : private boost::noncopyable
{
private:
	ComPtr<IOPCServer> server;
	Bool is_connected;
	String server_id;
	String host_name;

	void connectToRemoteServer( CLSID cClsid );
	CLSID getCLSID();
public:
	FRL_EXCEPTION_CLASS( AlreadyConnection );
	FRL_EXCEPTION_CLASS( NotResolveProgID );
	FRL_EXCEPTION_CLASS( CreateServerObjectError );
	FRL_EXCEPTION_CLASS( QueryInterfaceError );
	FRL_EXCEPTION_CLASS( NotConnected );
	FRL_EXCEPTION_CLASS( UnknownError );

	ServerConnection( const String& server_id_, const String& host_name_ );
	~ServerConnection();

	void connect();
	Bool isConnected();
	OPCSERVERSTATE getServerState();
	Bool isInterfaceSupported( const IID &iid );
	const String& getServerID();
	const String& getHostName();
	void addGroupAsyncIO2( const String& group_name );

}; // class ServerConnection

typedef boost::shared_ptr< ServerConnection > ServerConnectionPtr;

} // namespace client
} // namespace da
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
#endif // frl_opc_da_client_server_connection_h_
