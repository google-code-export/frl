#ifndef frl_opc_da_client_server_connection_h_
#define frl_opc_da_client_server_connection_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <map>
#include <vector>
#include "../dependency/vendors/opc_foundation/opcda.h"
#include "../dependency/vendors/opc_foundation/opccomn.h"
#include "frl_smart_ptr.h"
#include "frl_exception.h"
#include "opc/da/client/frl_opc_da_client_group_base.h"

namespace frl{ namespace opc{ namespace da{ namespace client{

class ServerConnection : private boost::noncopyable
{
private:
	ComPtr<IOPCServer> server;
	ComPtr<IOPCCommon> common;
	ComPtr<IOPCBrowse> browse;
	ComPtr<IOPCItemIO> item_io;
	Bool is_connected;
	String server_id;
	String host_name;
	boost::mutex scope_guard;

	typedef std::map< String, GroupPtr > GroupList;
	typedef std::pair< String, GroupPtr > GroupElemPair;
	GroupList group_list;

	void connectToRemoteServer( CLSID cClsid );
	CLSID getCLSID();
	void internalRemoveGroup( const String& name_, Bool force );
	void checkIsConnect();
public:
	FRL_EXCEPTION_CLASS( AlreadyConnection );
	FRL_EXCEPTION_CLASS( NotResolveProgID );
	FRL_EXCEPTION_CLASS( CreateServerObjectError );
	FRL_EXCEPTION_CLASS( QueryInterfaceError );
	FRL_EXCEPTION_CLASS( NotConnected );
	FRL_EXCEPTION_CLASS( UnknownError );
	FRL_EXCEPTION_CLASS( GroupNotExist );
	FRL_EXCEPTION_CLASS( GroupAlreadyExist );

	ServerConnection( const String& server_id_, const String& host_name_ );
	~ServerConnection();

	void connect();
	void disconnect();
	Bool isConnected();
	OPCSERVERSTATE getServerState();
	Bool isInterfaceSupported( const IID &iid );
	const String& getServerID();
	const String& getHostName();
	GroupPtr addGroupAsyncIO2( const String& group_name );
	GroupPtr getGroupByName( const String& name );
	void removeGroup( const String& name );
	void removeGroupForce( const String& name );
	std::vector< GroupPtr > getGoupList();
	Bool testComplianceOPC_DA1();
	Bool testComplianceOPC_DA2();
	Bool testComplianceOPC_DA3();
}; // class ServerConnection

typedef boost::shared_ptr< ServerConnection > ServerConnectionPtr;

} // namespace client
} // namespace da
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
#endif // frl_opc_da_client_server_connection_h_
