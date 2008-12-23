#include "opc/da/frl_opc_da_server_browser.h"

int main( int argc, char*argv[] )
{
	CoInitialize( NULL );
	std::vector<frl::String> list;
	frl::opc::da::ServerBrowser::getServerListDA2( list );

	return 0;
}
