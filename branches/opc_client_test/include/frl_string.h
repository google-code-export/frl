#ifndef frl_string_h_
#define frl_string_h_

#include "frl_types.h"
#include "stream_std/frl_sstream.h"

namespace frl{

// Return string length
size_t stringLength(const frl::Char* str );
size_t stringLength( const frl::String& string );

// Converting string to wstring
std::wstring string2wstring( const std::string& str );

// Converting wstring to string
std::string wstring2string( const std::wstring& wstr );

// Always return wstring
std::wstring unicodeCompatibility( const std::wstring& str );
std::wstring unicodeCompatibility( const std::string& str );

// Always return multi byte string
std::string multiByteCompatibility( const std::string& str );
std::string multiByteCompatibility( const std::wstring& str );

// Always return String == FRL_CHARACTER
String similarCompatibility( const std::string& str );
String similarCompatibility( const std::wstring& str );

// Convert number to string
template < class T >
String decNumber2String( T number )
{
	frl::stream_std::StringStream ss;
	ss << std::dec << number;
	return ss.str();
}

template< class T >
String octNumber2String( T number )
{
	frl::stream_std::StringStream ss;
	ss << std::oct << number;
	return ss.str();
}

template< class T >
String hexNumber2String( T number )
{
	frl::stream_std::StringStream ss;
	ss << std::hex << number;
	return ss.str();
}

Bool removeSymbolsFromStart( String& str, Char symbol );	
Bool removeSymbolsFromEnd( String& str, Char symbol );
Bool removeSymbols( String& str, Char symbol );
size_t getCountsSymbol( const String& str, Char symbol, size_t fromPos_ = 0, size_t toPos_ = 0 );

} // FatRat Library

#endif // frl_string_h_
