#ifndef frl_opc_da_client_h_
#define frl_opc_da_client_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include "frl_opc.h"
#include "frl_exception.h"

namespace frl{ namespace opc{ namespace da{

class Client
{
private:
	Bool is_connected;
	frl::ComPtr<IOPCServer> server;

public:
	FRL_EXCEPTION_CLASS( AlreadyConnection );
	FRL_EXCEPTION_CLASS( NotResolveProgID );
	FRL_EXCEPTION_CLASS( CreateServerObjectError );
	FRL_EXCEPTION_CLASS( QueryInterfaceError );

	Client();
	void Connect( const String &to_server_id, const String &to_host = FRL_STR("") );
	Bool IsConnected();
}; // class Client

} // namespace da
} // namespace opc
} // namespace frl

#endif // FRL_PLATFORM
#endif // frl_opc_da_client_h_
