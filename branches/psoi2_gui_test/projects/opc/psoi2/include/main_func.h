#ifndef main_func_h_
#define main_func_h_
#include <Windows.h>

// Saves instance handle and creates main window
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

// Registers the window class.
BOOL RegClass( HINSTANCE hInstance );

// Processes messages for the main window.
LRESULT CALLBACK MainFunc( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param );

#endif // main_func_h_
