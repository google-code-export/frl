#include <Windows.h>
#include "console_std/frl_iostream.h"
#include "psoi2_device_manager.h"
#include "psoi2_device.h"
#include "main_func.h"
#include "global_var.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	MSG msg;
	{
		HWND h_tmp = ::FindWindow( global::main_class_name, global::main_window_title );
		if( h_tmp )
		{
			if( ::IsIconic( h_tmp ) )
				ShowWindow( h_tmp, SW_RESTORE );
			SetForegroundWindow( h_tmp );
			return 0;
		}
	}

	if( ! RegClass( hInstance ) )
	{
		frl::String msg_string = frl::sys::util::getLastErrorDescription();
		MessageBox( NULL, msg_string.c_str(), FRL_STR("PSOI2 OPC server: Main window registration error"), MB_OK | MB_ICONSTOP );
		return FALSE;
	}

	#if( FRL_CHARACTER == FRL_CHARACTER_UNICODE )
		frl::String cmd_line = frl::string2wstring( lpCmdLine );
	#else
	frl::String cmd_line = lpCmdLine;
	#endif
	if( cmd_line.length() )
	{
		using namespace frl::console_std;
		if( frl::String( FRL_STR("-r") ) == cmd_line )
		{
			try
			{
				psoi2util::regServer();
				MessageBox( NULL, FRL_STR("Registration success!"), FRL_STR("PSOI2 OPC server: Registration message"), MB_OK | MB_ICONINFORMATION );
			}
			catch( frl::Exception &ex )
			{
				frl::String msg = FRL_STR("Registration fail!\n");
				msg += ex.getFullDescription();
				MessageBox( NULL, msg.c_str(), FRL_STR("PSOI2 OPC server: Registration message"), MB_OK | MB_ICONSTOP );
			}
			catch( ... )
			{
				MessageBox( NULL, FRL_STR("Registration fail!\nUnknown error"), FRL_STR("PSOI2 OPC server: Registration message"), MB_OK | MB_ICONSTOP );
			}
			return 0;
		}

		if( frl::String( FRL_STR("-u") ) == cmd_line )
		{
			try
			{
				psoi2util::unregServer();
				MessageBox( NULL, FRL_STR("Removing registration success!"), FRL_STR("PSOI2 OPC server: Registration message"), MB_OK | MB_ICONINFORMATION );
			}
			catch( frl::Exception &ex )
			{
				frl::String msg = FRL_STR("Unregistration fail!\n");
				msg += ex.getFullDescription();
				MessageBox( NULL, msg.c_str(), FRL_STR("Registration message"), MB_OK | MB_ICONSTOP );
			}
			catch( ... )
			{
				MessageBox( NULL, FRL_STR("Unregistration fail!\nUnknown error"), FRL_STR("PSOI2 OPC server: Registration message"), MB_OK | MB_ICONSTOP );
			}
			return 0;
		}

		if( frl::String( FRL_STR("-Embedding") ) != cmd_line )
		{
			MessageBox( NULL,
								FRL_STR("Available parameters:\n\"-r\" - Registration server.\n\"-u\"- Unregistration server."),
								FRL_STR("PSOI2 OPC server: Registration message"),
								MB_OK | MB_ICONHAND );
			return 0;
		}
	}
	
	global::devManager::getInstance();

	Bool exitOnAllClientDisconnected = True;
	{
		frl::poor_xml::Document config;
		try
		{
			config.loadFromCurrenttDir( FRL_STR( "config.xml" ) );
			frl::String sw = config.getRoot()->getProprtyVal( FRL_STR("ShowWindow") );
			if( frl::lexicalCast<String, int>( sw ) == 0 )
			{
				InitInstance( hInstance, SW_HIDE );
			}
			else
			{
				InitInstance( hInstance, nShowCmd );
			}
			frl::String isExit = config.getRoot()->getProprtyVal( FRL_STR("ExitOnAllClientDisconnected") );
			exitOnAllClientDisconnected = frl::lexicalCast< String, Bool >( isExit );
		}
		catch( frl::Exception& ){}
	}

	if( exitOnAllClientDisconnected )
	{
		Sleep( 20000 ); // wait for first client
		while( GetMessage( &msg, 0, 0, 0 ) && frl::opc::factory.isServerInUse() )
			DispatchMessage( &msg );
	}
	else
	{
		while( GetMessage( &msg, 0, 0, 0 ) )
			DispatchMessage( &msg );
	}

	return msg.wParam;
}
