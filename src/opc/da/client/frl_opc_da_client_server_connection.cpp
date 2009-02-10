#include "opc/da/client/frl_opc_da_client_server_connection.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include <boost/foreach.hpp>
#include <boost/mem_fn.hpp>
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
	if( is_connected )
	{
		disconnect();
	}

}

void ServerConnection::connect()
{
	FRL_EXCEPT_GUARD();
	boost::mutex::scoped_lock guard( scope_guard );
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

void ServerConnection::disconnect()
{
	FRL_EXCEPT_GUARD();
	boost::mutex::scoped_lock guard( scope_guard );
	if( ! isConnected() )
		FRL_THROW_S_CLASS( NotConnected );

	BOOST_FOREACH( GroupElemPair gr, group_list )
	{
		gr.second->removeGroup( False );
	}
}

frl::Bool ServerConnection::isConnected()
{
	return is_connected;
}

OPCSERVERSTATE ServerConnection::getServerState()
{
	FRL_EXCEPT_GUARD();
	OPCSERVERSTATUS *status = getStatus();
	OPCSERVERSTATE tmp = status->dwServerState;
	os::win32::com::freeMemory( status->szVendorInfo );
	os::win32::com::freeMemory( status );
	return tmp;
}

OPCSERVERSTATUS* ServerConnection::getStatus()
{
	FRL_EXCEPT_GUARD();
	boost::mutex::scoped_lock guard( scope_guard );
	checkIsConnect();
	OPCSERVERSTATUS *status = NULL;
	HRESULT result = server->GetStatus( &status );
	if( FAILED( result ) )
		FRL_THROW_OPC( result );
	return status;
}

Bool ServerConnection::isInterfaceSupported( const IID &iid )
{
	boost::mutex::scoped_lock guard( scope_guard );
	checkIsConnect();
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

	server_info.pwszName = util::duplicateString( unicodeCompatibility( host_name ) );
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

GroupPtr ServerConnection::addGroupAsyncIO2( const String& group_name )
{
	FRL_EXCEPT_GUARD();
	boost::mutex::scoped_lock guard( scope_guard );
	checkIsConnect();
	GroupList::iterator it = group_list.find( group_name );
	if( it != group_list.end() )
		FRL_THROW_S_CLASS( GroupAlreadyExist );
	AsyncIO2Group *new_gr = new AsyncIO2Group( group_name, server );
	new_gr->create();
	GroupPtr new_group( new_gr );
	GroupElemPair new_elem( group_name, new_group );
	group_list.insert( new_elem );
	return new_group; 
}

GroupPtr ServerConnection::getGroupByName( const String& group_name )
{
	FRL_EXCEPT_GUARD();
	boost::mutex::scoped_lock guard( scope_guard );
	checkIsConnect();
	GroupList::iterator it = group_list.find( group_name );
	if( it == group_list.end() )
		FRL_THROW_S_CLASS( GroupNotExist );
	return it->second;
}

void ServerConnection::removeGroup( const String& group_name )
{
	FRL_EXCEPT_GUARD();
	internalRemoveGroup( group_name, False );
}

void ServerConnection::removeGroupForce( const String& group_name )
{
	FRL_EXCEPT_GUARD();
	internalRemoveGroup( group_name, True );
}

void ServerConnection::internalRemoveGroup( const String& group_name, Bool force )
{
	FRL_EXCEPT_GUARD();
	boost::mutex::scoped_lock guard( scope_guard );
	checkIsConnect();
	GroupList::iterator it = group_list.find( group_name );
	if( it == group_list.end() )
		FRL_THROW_S_CLASS( GroupNotExist );
	it->second->removeGroup( force );
	group_list.erase( it );
}

frl::Bool ServerConnection::testComplianceOPC_DA1()
{
	FRL_EXCEPT_GUARD();
	if( ! isInterfaceSupported( IID_IUnknown ) )
		return False;
	if( ! isInterfaceSupported( IID_IOPCServer ) )
		return False;
	return True;
}

frl::Bool ServerConnection::testComplianceOPC_DA2()
{
	FRL_EXCEPT_GUARD();
	if( ! isInterfaceSupported( IID_IUnknown ) )
		return False;
	if( ! isInterfaceSupported( IID_IOPCServer ) )
		return False;
	if( ! isInterfaceSupported( IID_IOPCCommon ) )
		return False;
	if( ! isInterfaceSupported( IID_IConnectionPointContainer ) )
		return False;
	if( ! isInterfaceSupported( IID_IOPCItemProperties ) )
		return False;
	return True;
}

frl::Bool ServerConnection::testComplianceOPC_DA3()
{
	FRL_EXCEPT_GUARD();
	if( ! isInterfaceSupported( IID_IUnknown ) )
		return False;
	if( ! isInterfaceSupported( IID_IOPCServer ) )
		return False;
	if( ! isInterfaceSupported( IID_IOPCCommon ) )
		return False;
	if( ! isInterfaceSupported( IID_IConnectionPointContainer ) )
		return False;
	if( ! isInterfaceSupported( IID_IOPCBrowse ) )
		return False;
	if( ! isInterfaceSupported( IID_IOPCItemIO ) )
		return False;
	return True;
}

void ServerConnection::checkIsConnect()
{
	FRL_EXCEPT_GUARD();
	if( ! isConnected() )
		FRL_THROW_S_CLASS( NotConnected );
}

std::vector< GroupPtr > ServerConnection::getGoupList()
{
	FRL_EXCEPT_GUARD();
	boost::mutex::scoped_lock guard( scope_guard );
	if( ! isConnected() )
		FRL_THROW_S_CLASS( NotConnected );
	std::vector<GroupPtr> vec_tmp( group_list.size() );
	std::transform( group_list.begin(), group_list.end(), vec_tmp.begin(), boost::mem_fn(&GroupList::value_type::second ) );
	return vec_tmp;
}

frl::String ServerConnection::getServerErrorString( HRESULT error_id )
{
	FRL_EXCEPT_GUARD();
	boost::mutex::scoped_lock guard( scope_guard );
	checkIsConnect();
	ComPtr<IOPCCommon> comm; 
	getInterface( IID_IOPCCommon, comm );
	LPWSTR str_tmp;
	comm->GetErrorString( error_id, &str_tmp );
	String ret_str = similarCompatibility( str_tmp );
	os::win32::com::freeMemory( str_tmp );
	return ret_str;
}

std::vector< String > ServerConnection::getBranchesList( const String& from_branch,
																					const String& name_filter,
																					const String& vendor_filter )
{
	FRL_EXCEPT_GUARD();
	boost::mutex::scoped_lock guard( scope_guard );
	return getItemsList( OPC_BROWSE_FILTER_BRANCHES, from_branch, name_filter, vendor_filter );
}

std::vector< String > ServerConnection::getLeafsList( const String& from_branch,
																			const String& name_filter,
																			const String& vendor_filter )
{
	FRL_EXCEPT_GUARD();
	boost::mutex::scoped_lock guard( scope_guard );
	return getItemsList( OPC_BROWSE_FILTER_ITEMS, from_branch, name_filter, vendor_filter );
}

std::vector< String > ServerConnection::getItemsList( OPCBROWSEFILTER type, const String& from_branch, const String& name_filter, const String& vendor_filter )
{
	FRL_EXCEPT_GUARD();
	checkIsConnect();
	ComPtr<IOPCBrowse> br_ptr;
	getInterface( IID_IOPCBrowse, br_ptr );
	LPWSTR item_id = util::duplicateString( unicodeCompatibility( from_branch ) );
	LPWSTR cont_pint = NULL;
	LPWSTR name_filter_str = util::duplicateString( unicodeCompatibility( name_filter ) );
	LPWSTR vendor_filter_str = util::duplicateString( unicodeCompatibility( vendor_filter ) );
	DWORD *property_ids = os::win32::com::allocMemory<DWORD>();
	DWORD count;
	OPCBROWSEELEMENT *browse_elements;
	BOOL m = FALSE;
	HRESULT val = br_ptr->Browse( item_id, &cont_pint, 0, type, name_filter_str, vendor_filter_str, FALSE, FALSE, 0, property_ids, &m, &count, &browse_elements );
	if( FAILED( val ) )
	{
		os::win32::com::freeMemory( property_ids );
		os::win32::com::freeMemory( item_id );
		os::win32::com::freeMemory( name_filter_str );
		os::win32::com::freeMemory( vendor_filter_str );
		FRL_THROW_OPC( val );
	}

	std::vector< String > ret_vec( count );
	for( DWORD i = 0; i < count; ++i )
	{
		ret_vec[i] = similarCompatibility( browse_elements[i].szName );
		os::win32::com::freeMemory( browse_elements[i].szName );
		os::win32::com::freeMemory( browse_elements[i].szItemID );
		os::win32::com::freeMemory( browse_elements[i].ItemProperties.pItemProperties );
	}
	os::win32::com::freeMemory( property_ids );
	os::win32::com::freeMemory( item_id );
	os::win32::com::freeMemory( name_filter_str );
	os::win32::com::freeMemory( vendor_filter_str );
	os::win32::com::freeMemory( browse_elements );
	return ret_vec;
}

} // namespace client
} // namespace da
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
