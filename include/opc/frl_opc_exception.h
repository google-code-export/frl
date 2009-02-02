#ifndef frl_opc_exception_h_
#define frl_opc_exception_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include "frl_exception.h"
#include "opc/frl_opc_util.h"

namespace frl{ namespace opc{ 

class OpcException : public Exception
{
public:

	OpcException( DWORD error_code, const frl::String &function_, const frl::String &file_, frl::ULong line_)
		: Exception( FRL_STR(""), function_, file_, line_ )
	{
		util::getErrorString( error_code, LOCALE_SYSTEM_DEFAULT, description );
	}

	OpcException( DWORD error_code, const frl::String &description_, const frl::String &function_, const frl::String &file_, frl::ULong line_)
		: frl::Exception( description_, function_, file_, line_ )
	{
		String msg;
		util::getErrorString( error_code, LOCALE_SYSTEM_DEFAULT, msg );
		description += FRL_STR(". ");
		description += msg;
	}

	virtual ~OpcException() throw() {}
};

#define FRL_THROW_OPC( error_code )\
	throw( frl::opc::OpcException( error_code, FRL_FUNCTION_NAME, FRL_FILE_NAME, __LINE__ ) );

#define FRL_THROW_OPC_EX( error_code, description )\
	throw( frl::opc::OpcException( error_code, description, FRL_FUNCTION_NAME, FRL_FILE_NAME, __LINE__ ) );

} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
#endif // frl_opc_exception_h_
