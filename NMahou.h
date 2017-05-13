#ifndef _NMMHeader // NATIVE MAHOU MAIN HEADER
	#define _NMMHeader
	#include "inc/list.h"
	#define _O_U16TEXT 0x20000
	#ifndef UNICODE
		#define UNICODE
	#endif
	#ifndef _UNICODE
		#define _UNICODE
	#endif
	wchar_t* NM_VERSION;
	//Main Handle of Main Window
	HWND NMMainHWND;
	HINSTANCE NMInstance;
	NOTIFYICONDATA NMTrayIcon;
	HMENU NMTrayMenu;
	//Main list of word keys
	list_t* c_word;
	//Main list of line keys
	list_t* c_line;
	//List of all layouts
	list_t* a_layouts;
	//Modifiers
	bool Rshift, Ralt, Rctrl, Rwin, Lshift, Lalt, Lctrl, Lwin;
	//Main 2 layouts
	unsigned int LAYOUT1;
	unsigned int LAYOUT2;
	//If NMahou input.
	bool SELF = false;
#endif