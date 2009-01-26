#include "opc/da/frl_opc_da_client.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )

namespace frl{ namespace opc { namespace da{

Client::Client()
	: is_connected( False )
{

}

void Client::Connect( const String &to_server_id, const String &to_host )
{
	if( IsConnected() )
		FRL_THROW_S_CLASS( AlreadyConnection );
	
	// find CLSID
	CLSID cClsid = GUID_NULL;
	if( FAILED(CLSIDFromProgID( to_server_id.c_str(), &cClsid)) )
	{
		if( UuidFromString( (unsigned short*)to_server_id.c_str(), &cClsid) != RPC_S_OK )
			FRL_THROW_S_CLASS( NotResolveProgID );
	}

	// trying connection to server
	if( to_host.empty() ) // if local server
	{
		if( FAILED( CoCreateInstance( cClsid, NULL, CLSCTX_ALL, IID_IOPCServer, (void**)&server ) ) )
			FRL_THROW_S_CLASS( CreateServerObjectError );
	}
	else // if remote server
	{
		COSERVERINFO server_info;
		os::win32::com::zeroMemory( &server_info );
		server_info.pwszName = util::duplicateString( to_host );

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

frl::Bool Client::IsConnected()
{
	return is_connected;
}

OPCSERVERSTATE Client::getServerStatus()
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

} // namespace da
} // namespace opc
} // namespace frl

#endif // FRL_PLATFORM_WIN32
