#include <Windows.h>
#include <commctrl.h>
#include "main_func.h"
#include "global_var.h"
#include "resource.h"
#include "psoi2_device.h"
#include "prop_func.h"

#define MSG_NOTIFYICON (WM_APP) + 100

void systrayIcon( HINSTANCE hinstance, HWND hwnd, bool add )
{
	NOTIFYICONDATA nid;
	ZeroMemory( &nid,sizeof(nid) );
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = 0;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = MSG_NOTIFYICON;
	nid.hIcon = LoadIcon( hinstance, MAKEINTRESOURCE(IDI_ICON_MAIN) );
	lstrcpy( nid.szTip,TEXT("Double-click to maxmize!") );

	if( add )
		Shell_NotifyIcon( NIM_ADD, &nid );
	else
		Shell_NotifyIcon( NIM_DELETE, &nid );
}


BOOL RegClass( HINSTANCE hInstance )
{
	WNDCLASS wc;
	wc.hbrBackground	= (HBRUSH)( COLOR_WINDOW );
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainFunc;
	wc.hInstance = hInstance;
	wc.lpszClassName = global::main_class_name;
	wc.lpszMenuName = NULL;
	wc.hIcon = LoadIcon( hInstance, (LPCTSTR)IDI_ICON_MAIN );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	return RegisterClass( &wc );
}

BOOL createWindow()
{
	int x_screen = ::GetSystemMetrics( SM_CXSCREEN );
	int y_screen = ::GetSystemMetrics( SM_CYSCREEN );
	y_screen = y_screen - ::GetSystemMetrics( SM_CYMENU );

	x_screen = x_screen - global::main_width - 10;
	y_screen = y_screen - global::main_height - 10;

	global::main_hwnd = CreateWindow( 
		global::main_class_name, 
		global::main_window_title,
		WS_VISIBLE | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
		x_screen,
		y_screen,
		global::main_width,
		global::main_height,
		NULL,
		NULL,
		global::main_hinstance,
		NULL );

	if( ! global::main_hwnd )
		return FALSE;

	return TRUE;
}

BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	global::main_hinstance = hInstance;

	if( ! createWindow() )
		return FALSE;

	::ShowWindow( global::main_hwnd, nCmdShow );

	if( nCmdShow == SW_HIDE )
		systrayIcon( global::main_hinstance, global::main_hwnd, true );
	else
		::UpdateWindow( global::main_hwnd );

	return TRUE;
}

LRESULT CALLBACK MainFunc( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param )
{
	switch( msg )
	{
		case WM_SIZE:
		{
			if( w_param == SIZE_MINIMIZED )
			{
				systrayIcon( global::main_hinstance, hwnd, true );
				ShowWindow( hwnd, SW_HIDE );
			}
		}
		break;

		case MSG_NOTIFYICON:
		{
			if( l_param ==  WM_LBUTTONDBLCLK )
			{
				systrayIcon( global::main_hinstance, hwnd, false );
				ShowWindow( hwnd, SW_RESTORE );
				SetForegroundWindow( hwnd );
			}
		}
		break;

		case WM_CREATE:
		{
			InitCommonControls();

			global::tree_hwnd = CreateWindowEx( 
				NULL,
				WC_TREEVIEW,
				NULL,
				WS_CHILD+WS_VISIBLE+TVS_HASLINES+TVS_HASBUTTONS+TVS_LINESATROOT,
				5, 5,
				210,
				410, 
				hwnd,
				(HMENU)ID_MAIN_TREE,
				global::main_hinstance, NULL); // Создание tree view

			HIMAGELIST hImageList=ImageList_Create( 16,16,ILC_COLOR24,3, 10 );
			HBITMAP hBitMap=LoadBitmap( global::main_hinstance, MAKEINTRESOURCE(IDB_TREE_LIST) );
			ImageList_Add(hImageList,hBitMap,NULL);
			DeleteObject(hBitMap);
			SendDlgItemMessage( hwnd,ID_MAIN_TREE,TVM_SETIMAGELIST,0,(LPARAM)hImageList);

			std::vector< Psoi2Device* >::const_iterator end = global::devManager::getInstance().getDevices().end();
			std::vector< Psoi2Device* >::const_iterator it = global::devManager::getInstance().getDevices().begin();

			// temporary variables
			TV_INSERTSTRUCT tv_struct;
			HTREEITEM parent = 0;
			HTREEITEM item_handle = 0;
			frl::String ch_tmp;

			for( ; it != end; ++it )
			{
				frl::String tpm = FRL_STR("COM_");
				tpm += frl::lexicalCast< frl::UInt, frl::String >( (*it)->getPortNumber() );
				tpm += FRL_STR(": ");

				tpm += frl::lexicalCast< frl::UInt, frl::String >( (*it)->getChannelsNumber() );
				tpm += FRL_STR(" channels");

				if( (*it)->isSimulation() )
					tpm += FRL_STR(" (simulation!)");

				tv_struct.hParent=NULL;
				tv_struct.hInsertAfter=TVI_ROOT;
				tv_struct.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
				tv_struct.item.pszText= (frl::Char*)tpm.c_str();
				tv_struct.item.iImage=0;
				tv_struct.item.iSelectedImage = 0;
				parent = (HTREEITEM)SendDlgItemMessage( hwnd, ID_MAIN_TREE, TVM_INSERTITEM, 0, (LPARAM)&tv_struct );
				
				for( frl::UInt i = 0; i < (*it)->getChannelsNumber(); ++i )
				{
					tv_struct.hParent=parent;
					tv_struct.hInsertAfter=TVI_LAST;
					ch_tmp = FRL_STR( "Cannel_" );
					ch_tmp += frl::lexicalCast< frl::Int, frl::String >( i );
					tv_struct.item.pszText=(frl::Char*)ch_tmp.c_str();
					tv_struct.item.iImage=2;
					tv_struct.item.iSelectedImage=1;
					item_handle = (HTREEITEM)SendDlgItemMessage( hwnd, ID_MAIN_TREE, TVM_INSERTITEM,0,(LPARAM)&tv_struct);
					global::channels_map::getInstance().insert( std::make_pair( item_handle,global::ChannelDescr( (*it)->getPortNumber(), i ) ) );
				}
			}

			return 0;
		}
		break;

		case WM_CLOSE:
		{
			int ret_val = MessageBox( hwnd, FRL_STR("Do you really want to exit?"), FRL_STR("PSOI2 OPC server: Confirmation message"), MB_ICONQUESTION | MB_YESNO );
			if( ret_val == IDNO )
			{
				return 0;
			}
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return 0;		
		}
		break;

		case WM_NOTIFY:
		{
			case ID_MAIN_TREE:
			{
				if( ((LPNMHDR)l_param)->code == NM_DBLCLK)
				{
					using namespace global;
					HTREEITEM Selected = NULL;
					Selected=(HTREEITEM)SendDlgItemMessage(hwnd,ID_MAIN_TREE,TVM_GETNEXTITEM,TVGN_CARET,(LPARAM)Selected);
					DialogBoxParam( global::main_hinstance, MAKEINTRESOURCE( IDD_CHANNEL_PROP ), hwnd, (DLGPROC)PropFunc, (LPARAM)Selected );
					return 0;
				}
			}
			return 0;
		}
		break;

		case TVN_ITEMEXPANDED:
		{
			return 0;
		}
		break;
	}
	return DefWindowProc( hwnd, msg, w_param, l_param );
}
