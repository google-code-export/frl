#ifndef frl_os_win32_exception_h_
#define frl_os_win32_exception_h_
#include "frl_platform.h"

#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include "frl_exception.h"
#include "sys/frl_sys_util.h"

namespace frl{ namespace os{ namespace win32{

class SystemException : public Exception
{
public:

	SystemException( const frl::String &function_, const frl::String &file_, frl::ULong line_)\
		: frl::Exception( sys::util::getLastErrorDescription(), function_, file_, line_ )
	{
	}

	SystemException( const frl::String &description_, const frl::String &function_, const frl::String &file_, frl::ULong line_)\
		: frl::Exception( description_ + sys::util::getLastErrorDescription(), function_, file_, line_ )
	{
	}

	SystemException( DWORD error_code, const frl::String &function_, const frl::String &file_, frl::ULong line_)\
		: frl::Exception( sys::util::getCodeErrorDescription( error_code ), function_, file_, line_ )
	{
	}

	SystemException( DWORD error_code, const frl::String &description_, const frl::String &function_, const frl::String &file_, frl::ULong line_)\
		: frl::Exception( description_ + sys::util::getCodeErrorDescription( error_code ), function_, file_, line_ )
	{
	}

	virtual ~SystemException() throw() {}
};

#define FRL_THROW_SYSAPI()\
	throw( frl::os::win32::SystemException( FRL_FUNCTION_NAME, FRL_FILE_NAME, __LINE__ ) );

#define FRL_THROW_SYSAPI_CODE( error_code )\
	throw( frl::os::win32::SystemException( FRL_FUNCTION_NAME, FRL_FILE_NAME, __LINE__ ) );

#define FRL_THROW_SYSAPI_EX( description )\
	throw( frl::os::win32::SystemException( description, FRL_FUNCTION_NAME, FRL_FILE_NAME, __LINE__ ) );

#define FRL_THROW_SYSAPI_CODE_EX( description, error_code )\
	throw( frl::os::win32::SystemException(error_code, description, FRL_FUNCTION_NAME, FRL_FILE_NAME, __LINE__ ) );

} // namespace win32
} // namespace os
} // namespace FatRat Library

#endif // FRL_PLATFORM_WIN32

#endif // frl_os_win32_exception_h_
