#include "opc/da/client/frl_opc_da_client_server_connection.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include "os/win32/com/frl_os_win32_com_allocator.h"
#include "opc/frl_opc_util.h"

namespace frl{ namespace opc{ namespace da{ namespace client{

ServerConnection::ServerConnection( const String& server_id_, const String& host_name_ )
	: is_connected( False ), server_id( server_id_ ), host_name ( host_name_ )
{
}

ServerConnection::~ServerConnection()
{
}

void ServerConnection::Connect()
{
	if( IsConnected() )
		FRL_THROW_S_CLASS( AlreadyConnection );

	// find CLSID
	CLSID cClsid = GUID_NULL;

	#if( FRL_CHARACTER == FRL_CHARACTER_UNICODE )
		if( FAILED(CLSIDFromProgID( server_id.c_str(), &cClsid)) )
		{
			if( UuidFromString( (unsigned short*)server_id.c_str(), &cClsid) != RPC_S_OK )
				FRL_THROW_S_CLASS( NotResolveProgID );
		}
	#else
		if( FAILED(CLSIDFromProgID( string2wstring( server_id ).c_str(), &cClsid)) )
		{
			if( UuidFromString( (unsigned char*)server_id.c_str(), &cClsid) != RPC_S_OK )
				FRL_THROW_S_CLASS( NotResolveProgID );
		}	
	#endif // FRL_CHARACTER_UNICODE

	// trying connection to server
	if( host_name.empty() || host_name == FRL_STR("localhost") ) // if local server
	{
		if( FAILED( CoCreateInstance( cClsid, NULL, CLSCTX_ALL, IID_IOPCServer, (void**)&server ) ) )
			FRL_THROW_S_CLASS( CreateServerObjectError );
	}
	else // if remote server
	{
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
		if( FAILED( CoCreateInstanceEx( cClsid,
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

	}// end trying connection
	is_connected = True;
}

frl::Bool ServerConnection::IsConnected()
{
	return is_connected;
}

OPCSERVERSTATE ServerConnection::getServerState()
{
	if( ! IsConnected() )
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
	if( ! IsConnected() )
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

} // namespace client
} // namespace da
} // namespace opc
} // namespace frl

#endif // FRL_PLATFORM_WIN32
