#ifndef frl_opc_da_client_browser_h_
#define frl_opc_da_client_browser_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )

#include <vector>
#include <COMCat.h>
#include "frl_types.h"

namespace frl{ namespace opc{ namespace da{ namespace client{

struct ServerInfo
{
	String prog_id;
	String clsid_str;
};

class ServerBrowser
{
private:
	static std::vector<ServerInfo> getLocalAvailableServers( const CATID &interface_ );
public:
	static std::vector<ServerInfo> getLocalRegistryAvailableServers();
	static std::vector<ServerInfo> getLocalAvailableServersDA1();
	static std::vector<ServerInfo> getLocalAvailableServersDA2();
	static std::vector<ServerInfo> getLocalAvailableServersDA3();
}; // class ServerBrowser

} // namespace client
} // namespace da
} // namespace opc
} // namespace FatRat Library

#endif // FRL_PLATFORM_WIN32
#endif // frl_opc_da_client_browser_h_
