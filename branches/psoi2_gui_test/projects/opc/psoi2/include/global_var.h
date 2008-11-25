#ifndef global_var_h_
#define global_var_h_
#include <Windows.h>
#include <CommCtrl.h>
#include <map>
#include "psoi2_device_manager.h"
#include "frl_singleton.h"

namespace global
{
	struct ChannelDescr
	{
		frl::UInt portNumber;
		frl::UInt channelNumber;
		ChannelDescr()
			: portNumber( 0 ), channelNumber( 0 )
		{}
		ChannelDescr( frl::UInt port, frl::UInt channel )
			:	portNumber( port ), channelNumber( channel )
		{
		}

	};

	const static TCHAR main_class_name[] = FRL_STR("psoi2_main_window");
	static HINSTANCE main_hinstance;
	static HWND main_hwnd;
	const TCHAR main_window_title[] = FRL_STR("PSOI2 OPC");
	static HWND tree_hwnd;

	typedef frl::SingletonMeyers< DeviceManager > devManager;
	typedef frl::SingletonMeyers< std::map< HTREEITEM, ChannelDescr > > channels_map;
	
	static const int main_height = 425;
	static const int main_width = 225;
}

#endif // global_var_h_
