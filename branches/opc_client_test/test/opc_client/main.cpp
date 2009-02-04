#include <boost/foreach.hpp>

#include "opc/da/client/frl_opc_da_client_client.h"
#include "opc/da/client/frl_opc_da_client_server_browser.h"
#include "opc/da/client/frl_opc_da_client_async_io_2_group.h"
#include "console_std/frl_iostream.h"


int main( int argc, char*argv[] )
{
	using namespace frl;
	CoInitialize( NULL );

	std::vector<frl::String> list_quick_all;
	opc::da::client::ServerBrowser::getAllServerListForce( list_quick_all );
	console_std::Out << FRL_STR("Accessible OPC DA servers (quick):\n");
	BOOST_FOREACH( String server, list_quick_all )
	{
		console_std::Out << server << FRL_STR("\n");
	}
	console_std::Out << std::endl;

	std::vector<frl::String> list_da1;
	opc::da::client::ServerBrowser::getServerListDA1( list_da1 );
	console_std::Out << FRL_STR("Accessible OPC DA1 servers:\n");
	BOOST_FOREACH( String server, list_da1 )
	{
		console_std::Out << server << FRL_STR("\n");
	}
	console_std::Out << std::endl;

	std::vector<frl::String> list_da2;
	opc::da::client::ServerBrowser::getServerListDA2( list_da2 );
	console_std::Out << FRL_STR("Accessible OPC DA2 servers:\n");
	BOOST_FOREACH( String server, list_da2 )
	{
		console_std::Out << server << FRL_STR("\n");
	}
	console_std::Out << std::endl;

	std::vector<frl::String> list_da3;
	opc::da::client::ServerBrowser::getServerListDA3( list_da3 );
	console_std::Out << FRL_STR("Accessible OPC DA3 servers:\n");
	BOOST_FOREACH( String server, list_da3 )
	{
		console_std::Out << server << FRL_STR("\n");
	}

	std::vector<frl::String> list_all_force;
	opc::da::client::ServerBrowser::getAllServerListForce( list_all_force );
	console_std::Out << FRL_STR("Accessible OPC DA servers:\n");
	BOOST_FOREACH( String server, list_all_force )
	{
		console_std::Out << server << FRL_STR("\n");
	}

	console_std::Out << FRL_STR("\nConnect to local server\n");
	opc::da::client::Client client;
	opc::da::client::HostPtr localhost = client.addHost( FRL_STR("localhost") );
	opc::da::client::ServerConnectionPtr test_client = localhost->addConnection( FRL_STR("Serg Baburin.SERVER_TEST.0.1") );
	test_client->connect();

	console_std::Out << FRL_STR("Server status: ");
	console_std::Out << test_client->getServerState() << std::endl;
	
	console_std::Out << FRL_STR("Server compliance OPC DA 1.0: ");
	console_std::Out << test_client->testComplianceOPC_DA1() << std::endl;

	console_std::Out << FRL_STR("Server compliance OPC DA 2.0: ");
	console_std::Out << test_client->testComplianceOPC_DA2() << std::endl;

	console_std::Out << FRL_STR("Server compliance OPC DA 3.0: ");
	console_std::Out << test_client->testComplianceOPC_DA3() << std::endl;

	console_std::Out << FRL_STR("Support IID_IOPCBrowseServerAddressSpace: ");
	console_std::Out << test_client->isInterfaceSupported( IID_IOPCBrowseServerAddressSpace ) << std::endl;

	opc::da::client::ServerConnectionPtr my_srv = localhost->getConnection( FRL_STR("Serg Baburin.SERVER_TEST.0.1") );

	my_srv->getServerErrorString( 0 );

	using namespace frl::opc::da::client;
	my_srv->addGroupAsyncIO2( FRL_STR("test_group") );
	try
	{
		my_srv->addGroupAsyncIO2( FRL_STR("test_group") );
	}
	catch( frl::Exception &ex )
	{
		ex.~Exception();
	}

	GroupPtr gr_ptr = my_srv->getGroupByName( FRL_STR("test_group") );
	std::vector< GroupPtr > gr_list = my_srv->getGoupList();
	my_srv->removeGroup( gr_ptr->getName() );
	try
	{
		my_srv->removeGroup( gr_ptr->getName() );	
	}
	catch( frl::Exception &ex )
	{
		ex.~Exception();
	}

	return 0;
 }
