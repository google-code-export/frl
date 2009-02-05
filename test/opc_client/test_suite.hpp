#ifndef opc_client_test_suite_h_
#define opc_client_test_suite_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include <boost/test/unit_test.hpp>
#include "../dependency/vendors/opc_foundation/opcerror.h"
#include "opc/da/client/frl_opc_da_client_client.h"
#include "opc/da/client/frl_opc_da_client_server_browser.h"
#include "opc/da/client/frl_opc_da_client_async_io_2_group.h"

BOOST_AUTO_TEST_SUITE( opc_client )

BOOST_AUTO_TEST_CASE( server_browser_get_server_list_force )
{
	std::vector<frl::String> list_all;
	BOOST_CHECK_NO_THROW( frl::opc::da::client::ServerBrowser::getAllServerListForce( list_all ) );
}

BOOST_AUTO_TEST_CASE( server_browser_get_server_list_da1 )
{
	std::vector<frl::String> list_all;
	CoInitialize( NULL );
	BOOST_CHECK_NO_THROW( frl::opc::da::client::ServerBrowser::getServerListDA1( list_all ) );
	CoUninitialize();
}

BOOST_AUTO_TEST_CASE( server_browser_get_server_list_da2 )
{
	CoInitialize( NULL );
	std::vector<frl::String> list_all;
	BOOST_CHECK_NO_THROW( frl::opc::da::client::ServerBrowser::getServerListDA2( list_all ) );
	CoUninitialize();
}

BOOST_AUTO_TEST_CASE( server_browser_get_server_list_da3 )
{
	CoInitialize( NULL );
	std::vector<frl::String> list_all;
	BOOST_CHECK_NO_THROW(frl::opc::da::client::ServerBrowser::getServerListDA3( list_all ) );
	CoUninitialize();
}

BOOST_AUTO_TEST_CASE( client_connect )
{
	CoInitialize( NULL );
	frl::opc::da::client::Client client;
	frl::opc::da::client::HostPtr localhost;
	BOOST_CHECK_NO_THROW( localhost = client.addHost( FRL_STR("localhost") ) );
	frl::opc::da::client::ServerConnectionPtr test_client;
	BOOST_CHECK_NO_THROW( test_client = localhost->addConnection( FRL_STR("Serg Baburin.SERVER_TEST.0.1") ) );
	BOOST_CHECK_NO_THROW( test_client->connect() );
	CoUninitialize();
}

BOOST_AUTO_TEST_CASE( client_get_server_status )
{
	CoInitialize( NULL );
	frl::opc::da::client::Client client;
	frl::opc::da::client::HostPtr localhost;
	BOOST_CHECK_NO_THROW( localhost = client.addHost( FRL_STR("localhost") ) );
	frl::opc::da::client::ServerConnectionPtr test_client;
	BOOST_CHECK_NO_THROW( test_client = localhost->addConnection( FRL_STR("Serg Baburin.SERVER_TEST.0.1") ) );
	BOOST_CHECK_NO_THROW( test_client->connect() );
	BOOST_CHECK_NO_THROW( BOOST_CHECK_EQUAL( test_client->getServerState(), OPC_STATUS_RUNNING ) );
	CoUninitialize();
}

BOOST_AUTO_TEST_CASE( client_check_opc_server_compliance )
{
	CoInitialize( NULL );
	frl::opc::da::client::Client client;
	frl::opc::da::client::HostPtr localhost;
	BOOST_CHECK_NO_THROW( localhost = client.addHost( FRL_STR("localhost") ) );
	frl::opc::da::client::ServerConnectionPtr test_client;
	BOOST_CHECK_NO_THROW( test_client = localhost->addConnection( FRL_STR("Serg Baburin.SERVER_TEST.0.1") ) );
	BOOST_CHECK_NO_THROW( test_client->connect() );
	BOOST_CHECK_NO_THROW( BOOST_CHECK( test_client->testComplianceOPC_DA1() ) );
	BOOST_CHECK_NO_THROW( BOOST_CHECK( test_client->testComplianceOPC_DA2() ) );
	BOOST_CHECK_NO_THROW( BOOST_CHECK( test_client->testComplianceOPC_DA3() ) );
	CoUninitialize();
}

BOOST_AUTO_TEST_CASE( client_is_interface_supported )
{
	CoInitialize( NULL );
	frl::opc::da::client::Client client;
	frl::opc::da::client::HostPtr localhost;
	BOOST_CHECK_NO_THROW( localhost = client.addHost( FRL_STR("localhost") ) );
	frl::opc::da::client::ServerConnectionPtr test_client;
	BOOST_CHECK_NO_THROW( test_client = localhost->addConnection( FRL_STR("Serg Baburin.SERVER_TEST.0.1") ) );
	BOOST_CHECK_NO_THROW( test_client->connect() );
	BOOST_CHECK_NO_THROW( BOOST_CHECK( test_client->isInterfaceSupported( IID_IOPCBrowseServerAddressSpace ) ) );
	CoUninitialize();
}

BOOST_AUTO_TEST_CASE( client_get_server_error_string )
{
	CoInitialize( NULL );
	frl::opc::da::client::Client client;
	frl::opc::da::client::HostPtr localhost;
	BOOST_CHECK_NO_THROW( localhost = client.addHost( FRL_STR("localhost") ) );
	frl::opc::da::client::ServerConnectionPtr test_client;
	BOOST_CHECK_NO_THROW( test_client = localhost->addConnection( FRL_STR("Serg Baburin.SERVER_TEST.0.1") ) );
	BOOST_CHECK_NO_THROW( test_client->connect() );
	BOOST_CHECK_NO_THROW( BOOST_CHECK_NO_THROW(
			test_client->getServerErrorString( OPC_E_INVALIDHANDLE ) == frl::String( FRL_STR( "The value of the handle is invalid.") ) ) );
	CoUninitialize();
}

BOOST_AUTO_TEST_CASE( client_add_get_remove_sync_io2_group )
{
	CoInitialize( NULL );
	frl::opc::da::client::Client client;
	frl::opc::da::client::HostPtr localhost;
	BOOST_CHECK_NO_THROW( localhost = client.addHost( FRL_STR("localhost") ) );
	frl::opc::da::client::ServerConnectionPtr test_client;
	BOOST_CHECK_NO_THROW( test_client = localhost->addConnection( FRL_STR("Serg Baburin.SERVER_TEST.0.1") ) );
	BOOST_CHECK_NO_THROW( test_client->connect() );
	frl::String gr_name = FRL_STR( "test_group" );
	BOOST_CHECK_NO_THROW( test_client->addGroupAsyncIO2( gr_name ) );
	BOOST_CHECK_THROW( test_client->addGroupAsyncIO2( gr_name ), frl::opc::da::client::ServerConnection::GroupAlreadyExist );
	frl::opc::da::client::GroupPtr gr_ptr;
	BOOST_CHECK_NO_THROW( gr_ptr = test_client->getGroupByName( gr_name ) );
	BOOST_CHECK_NO_THROW( test_client->removeGroup( gr_name ) );
	BOOST_CHECK_THROW( test_client->getGroupByName( gr_name ), frl::opc::da::client::ServerConnection::GroupNotExist );
	BOOST_CHECK_THROW( test_client->removeGroup( gr_name ), frl::opc::da::client::ServerConnection::GroupNotExist );
	CoUninitialize();
}

BOOST_AUTO_TEST_SUITE_END()

#endif // FRL_PLATFORM_WIN32
#endif // opc_client_test_suite_h_
