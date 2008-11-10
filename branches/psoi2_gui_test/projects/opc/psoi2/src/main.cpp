#include <Windows.h>
#include "console_std/frl_iostream.h"
#include "psoi2_device_manager.h"
#include "psoi2_device.h"
#include <WinCon.h>

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	frl::String cmd_line = frl::unicodeCompatibility( lpCmdLine );
	if( cmd_line.length() )
	{
		using namespace frl::console_std;
		if( frl::String( FRL_STR("-r") ) == cmd_line )
		{
			try
			{
				psoi2util::regServer();
				MessageBox( NULL, L"Registration success!", L"Registration message", MB_OK | MB_ICONINFORMATION );
			}
			catch( frl::Exception &ex )
			{
				frl::String msg = FRL_STR("Registration fail!\n");
				msg += ex.getFullDescription();
				MessageBox( NULL, msg.c_str(), L"Registration message", MB_OK | MB_ICONSTOP );
			}
			catch( ... )
			{
				MessageBox( NULL, L"Registration fail!\nUnknown error", L"Registration message", MB_OK | MB_ICONSTOP );
			}
			return 0;
		}

		if( frl::String( FRL_STR("-u") ) == cmd_line )
		{
			try
			{
				psoi2util::unregServer();
				MessageBox( NULL, L"Removing registration success!", L"Registration message", MB_OK | MB_ICONINFORMATION );
			}
			catch( frl::Exception &ex )
			{
				frl::String msg = FRL_STR("Unregistration fail!\n");
				msg += ex.getFullDescription();
				MessageBox( NULL, msg.c_str(), L"Registration message", MB_OK | MB_ICONSTOP );
			}
			catch( ... )
			{
				MessageBox( NULL, L"Unregistration fail!\nUnknown error", L"Registration message", MB_OK | MB_ICONSTOP );
			}
			return 0;
		}

		if( frl::String( FRL_STR("-Embedding") ) != cmd_line )
		{
			MessageBox( NULL,
								L"Available parameters:\n\"-r\" - Registration server.\n\"-u\"- Unregistration server.",
								L"Registration message",
								MB_OK | MB_ICONHAND );
			return 0;
		}
	}
	
	Bool exitOnAllClientDisconnected = True;
	{
		frl::poor_xml::Document config;
		try
		{
			config.loadFromCurrenttDir( FRL_STR( "config.xml" ) );
			frl::String sw = config.getRoot()->getProprtyVal( FRL_STR("ShowWindow") );
			if( frl::lexicalCast<String, int>( sw ) == 0 )
			{
				::FreeConsole();
			}
			else
			{
				frl::poor_xml::NodesList log = config.getRoot()->getSubNodes( FRL_STR("Log") );
				frl::String logLocale = (*log.begin())->getProprtyVal( FRL_STR("ConsoleLogLocale") );
				setlocale( LC_ALL, frl::multiByteCompatibility( logLocale ).c_str() );
			}
			frl::String isExit = config.getRoot()->getProprtyVal( FRL_STR("ExitOnAllClientDisconnected") );
			exitOnAllClientDisconnected = frl::lexicalCast< String, Bool >( isExit );
		}
		catch( frl::Exception& ){}
	}

	DeviceManager devManager;

	if( exitOnAllClientDisconnected )
	{
		Sleep( 20000 ); // wait for first client
		while( frl::opc::factory.isServerInUse() )
		{
			Sleep( 1000 );
		}
	}
	else
	{
		for( ; ; )
		{
			::Sleep( 1000 );
		}
	}

	return 0;
}
