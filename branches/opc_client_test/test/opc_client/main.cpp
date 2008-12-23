#include <boost/foreach.hpp>
#include "opc/da/frl_opc_da_server_browser.h"
#include "console_std/frl_iostream.h"
#include "stream_std/frl_ostream.h"

int main( int argc, char*argv[] )
{
	using namespace frl;
	CoInitialize( NULL );

	std::vector<frl::String> list_da1;
	opc::da::ServerBrowser::getServerListDA1( list_da1 );
	console_std::Out << FRL_STR("Accessible OPC DA1 servers:\n");
	BOOST_FOREACH( String i, list_da1 )
	{
		console_std::Out << i << FRL_STR("\n");
	}
	console_std::Out << std::endl;

	std::vector<frl::String> list_da2;
	opc::da::ServerBrowser::getServerListDA2( list_da2 );
	console_std::Out << FRL_STR("Accessible OPC DA2 servers:\n");
	BOOST_FOREACH( String i, list_da2 )
	{
		console_std::Out << i << FRL_STR("\n");
	}
	console_std::Out << std::endl;

	std::vector<frl::String> list_da3;
	opc::da::ServerBrowser::getServerListDA3( list_da3 );
	console_std::Out << FRL_STR("Accessible OPC DA3 servers:\n");
	BOOST_FOREACH( String i, list_da2 )
	{
		console_std::Out << i << FRL_STR("\n");
	}

	return 0;
}
