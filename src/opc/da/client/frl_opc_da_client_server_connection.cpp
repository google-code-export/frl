#include "opc/da/client/frl_opc_da_client_server_connection.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include "os/win32/com/frl_os_win32_com_allocator.h"
#include "opc/frl_opc_util.h"
#include "opc/da/client/frl_opc_da_client_async_io_2_group.h"

namespace frl{ namespace opc{ namespace da{ namespace client{

ServerConnection::ServerConnection( const String& server_id_, const String& host_name_ )
	: is_connected( False ), server_id( server_id_ ), host_name ( host_name_ )
{
}

ServerConnection::~ServerConnection()
{
}

void ServerConnection::connect()
{
	FRL_EXCEPT_GUARD();
	if( isConnected() )
		FRL_THROW_S_CLASS( AlreadyConnection );

	// find CLSID
	CLSID server_clsid = getCLSID();

	// trying connection to server
	if( host_name.empty() || host_name == FRL_STR("localhost") ) // if local server
	{
		if( FAILED( CoCreateInstance( server_clsid, NULL, CLSCTX_ALL, IID_IOPCServer, (void**)&server ) ) )
			FRL_THROW_S_CLASS( CreateServerObjectError );
	}
	else // if remote server
	{
		connectToRemoteServer( server_clsid );
	}// end trying connection
	is_connected = True;
}

frl::Bool ServerConnection::isConnected()
{
	return is_connected;
}

OPCSERVERSTATE ServerConnection::getServerState()
{
	FRL_EXCEPT_GUARD();
	if( ! isConnected() )
		FRL_THROW_S_CLASS( NotConnected );

	OPCSERVERSTATUS *status = NULL;
	if( FAILED( server->GetStatus( &status ) ) )
		FRL_THROW_S_CLASS( UnknownError );
	OPCSERVERSTATE tmp = status->dwServerState;
	os::win32::com::freeMemory( status->szVendorInfo );
	os::win32::com::freeMemory( status );
	return tmp;
}

frl::Bool ServerConnection::isInterfaceSupported( const IID &iid )
{
	if( ! isConnected() )
		FRL_THROW_S_CLASS( NotConnected );
	IUnknown *tmp;
	if( FAILED( server->QueryInterface( iid, (void**)&tmp ) ) )
		return False;
	tmp->Release();
	return True;
}

const String& ServerConnection::getServerID()
{
	return server_id;
}

const String& ServerConnection::getHostName()
{
	return host_name;
}

void ServerConnection::connectToRemoteServer( CLSID server_clsid )
{
	FRL_EXCEPT_GUARD();
	COSERVERINFO server_info;
	os::win32::com::zeroMemory( &server_info );

	#if( FRL_CHARACTER == FRL_CHARACTER_UNICODE  )
		server_info.pwszName = util::duplicateString( host_name );
	#else
		server_info.pwszName = util::duplicateString( string2wstring( host_name ) );
	#endif

	// setup requested interfaces
	MULTI_QI mq_result;
	os::win32::com::zeroMemory( &mq_result );

	mq_result.pIID = &IID_IOPCServer;
	mq_result.pItf = NULL;
	mq_result.hr   = S_OK;

	// call create instance
	if( FAILED( CoCreateInstanceEx( server_clsid,
		NULL,
		CLSCTX_REMOTE_SERVER,
		&server_info,
		1,
		&mq_result) ) )
	{
		FRL_THROW_S_CLASS( CreateServerObjectError );
	}

	// check QueryInterface result
	if( FAILED(mq_result.hr) )
		FRL_THROW_S_CLASS( QueryInterfaceError );

	// set pointer to server object
	ComPtr<IOPCServer> tmp( (IOPCServer*)mq_result.pItf );
	tmp.get()->Release(); // release tmp
	server.swap( tmp ); // equal: this->server->ptr = mq_result.pItf
}

CLSID ServerConnection::getCLSID()
{
	FRL_EXCEPT_GUARD();
	CLSID server_clsid = GUID_NULL;
	#if( FRL_CHARACTER == FRL_CHARACTER_UNICODE )
	if( FAILED(CLSIDFromProgID( server_id.c_str(), &server_clsid)) )
	{
		if( UuidFromString( (unsigned short*)server_id.c_str(), &server_clsid) != RPC_S_OK )
			FRL_THROW_S_CLASS( NotResolveProgID );
	}
	#else
	if( FAILED(CLSIDFromProgID( string2wstring( server_id ).c_str(), &server_clsid)) )
	{
		if( UuidFromString( (unsigned char*)server_id.c_str(), &server_clsid) != RPC_S_OK )
			FRL_THROW_S_CLASS( NotResolveProgID );
	}	
	#endif // FRL_CHARACTER_UNICODE
	return server_clsid;
}

ServerConnection::GroupElem ServerConnection::addGroupAsyncIO2( const String& group_name )
{
	AsyncIO2Group *new_gr = new AsyncIO2Group( group_name, server );
	new_gr->create();
	GroupElem new_group( new_gr );
	GroupElemPair new_elem( group_name, new_group );
	group_list.insert( new_elem );
	return new_group; 
}

ServerConnection::GroupElem ServerConnection::getGroup( const String& name )
{
	FRL_EXCEPT_GUARD();
	GroupList::iterator it = group_list.find( name );
	if( it == group_list.end() )
		FRL_THROW_S_CLASS( GroupNotExist );
	return it->second;
}

} // namespace client
} // namespace da
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
