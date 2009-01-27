#include "opc/da/client/frl_opc_da_client_client.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )

namespace frl{ namespace opc { namespace da{ namespace client {

Client::Client()
{
	CoInitialize( NULL );
}

Client::~Client()
{
	host_list.clear();
	CoUninitialize();
}

HostPtr Client::addHost( const String& to_host )
{
	HostListMap::iterator it = host_list.find( to_host );
	if( it != host_list.end() )
		FRL_THROW_S_CLASS( HostAlreadyAdded );
	HostPtr ptr( new Host( to_host ) );
	host_list.insert( std::make_pair( to_host, ptr ) );
	return ptr;
}

} // namespace client
} // namespace da
} // namespace opc
} // namespace frl

#endif // FRL_PLATFORM_WIN32
