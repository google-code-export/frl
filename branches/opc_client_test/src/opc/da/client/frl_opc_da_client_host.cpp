#include "opc/da/client/frl_opc_da_client_host.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include <boost/foreach.hpp>
#include "opc/da/client/frl_opc_da_client_server_connection.h"

namespace frl{ namespace opc{ namespace da{ namespace client{

Host::Host( const String& name_ )
: name( name_ )
{
}

Host::~Host()
{
	boost::mutex::scoped_lock guard( scope_guard );
	BOOST_FOREACH( ConnectionListElem cn, connection_list )
		cn.second->disconnect();
}

const String& Host::getName()
{
	return name;
}

ServerConnectionPtr Host::addConnection( const String& server_id )
{
	FRL_EXCEPT_GUARD();
	boost::mutex::scoped_lock guard( scope_guard );
	ConnectionList::iterator it = connection_list.find( server_id );
	if( it != connection_list.end() )
		FRL_THROW_S_CLASS( ConnectionAlreadyAdded );
	ServerConnectionPtr ptr( new ServerConnection( server_id, name ) );
	connection_list.insert( std::make_pair( server_id, ptr ) );
	return ptr;
}

ServerConnectionPtr Host::getConnection( const String& server_id )
{
	FRL_EXCEPT_GUARD();
	boost::mutex::scoped_lock guard( scope_guard );
	ConnectionList::iterator it = connection_list.find( server_id );
	if( it == connection_list.end() )
		FRL_THROW_S_CLASS( ConnectionNotFound );
	return it->second;
}

void Host::RemoveConnection( const String& server_id )
{
	FRL_EXCEPT_GUARD();
	boost::mutex::scoped_lock guard( scope_guard );
	ConnectionList::iterator it = connection_list.find( server_id );
	if( it == connection_list.end() )
		FRL_THROW_S_CLASS( ConnectionNotFound );
	it->second->disconnect();
	connection_list.erase( it );
}

} // namespace client
} // namespace da
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
