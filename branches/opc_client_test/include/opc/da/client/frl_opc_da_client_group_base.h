#ifndef frl_opc_da_client_group_base_h_
#define frl_opc_da_client_group_base_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include <boost/noncopyable.hpp>
#include "opc/da/client/frl_opc_da_client_group_info.h"

namespace frl{ namespace opc{ namespace da{ namespace client{

class GroupBase : private boost::noncopyable
{
private:
	void removeGroup( Bool force_ );

protected:
	GroupInfo info;

public:

	GroupBase( const String& name_, ComPtr<IOPCServer> server_ptr_ );
	virtual ~GroupBase();
	void create();
	void removeMe();
	void removeMeForce();
	void renameTo( const String& new_name );
	const String& getName();
};

} // namespace client
} // namespace da
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
#endif // frl_opc_da_client_group_base_h_
