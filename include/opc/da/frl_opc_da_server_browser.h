#ifndef frl_opc_da_client_browser_h_
#define frl_opc_da_client_browser_h_
#include <vector>
#include <COMCat.h>
#include "frl_types.h"

namespace frl{ namespace opc{ namespace da{

class ServerBrowser
{
private:
	static void getServerList( const CATID &interface_, std::vector<String> &to_list );
public:
	static void getAllServerListForce( std::vector<String> &to_list );
	static void getServerListDA1( std::vector<String> &to_list );
	static void getServerListDA2( std::vector<String> &to_list );
	static void getServerListDA3( std::vector<String> &to_list );
	static void getAllServerList( std::vector<String> &to_list );
}; // class ServerBrowser

} // namespace da
} // namespace opc
} // namespace FatRat Library

#endif // frl_opc_da_client_browser_h_
