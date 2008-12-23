#ifndef frl_opc_da_client_browser_h_
#define frl_opc_da_client_browser_h_
#include <vector>
#include "frl_types.h"

namespace frl{ namespace opc{ namespace da{

class ServerBrowser
{
private:
public:
	static void getServerListDA2( std::vector<frl::String> &to_list );
}; // class ServerBrowser

} // namespace da
} // namespace opc
} // namespace FatRat Library

#endif // frl_opc_da_client_browser_h_
