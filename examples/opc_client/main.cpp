#include "opc/da/client/frl_opc_da_client_client.h"

int main()
{
	frl::opc::da::client::Client examp_client;
	frl::opc::da::client::HostPtr localhost = examp_client.addHost( FRL_STR("localhost") );
	frl::opc::da::client::ServerConnectionPtr my_srv = localhost->addConnection( FRL_STR("Serg Baburin.SERVER_TEST.0.1") );
	my_srv->connect();
	std::vector< frl::String > branches = my_srv->getBranchesList( FRL_STR( "" ) );
	std::vector< frl::String > leafs = my_srv->getLeafsList( FRL_STR( "" ) );
	 return 0;
}
