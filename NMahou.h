#ifndef _NMMHeader // NATIVE MAHOU MAIN HEADER
	#define _NMMHeader
	#define _O_U16TEXT 0x20000
	#ifndef UNICODE
		#define UNICODE
	#endif
	#ifndef _UNICODE
		#define _UNICODE
	#endif
	#include <windows.h>
	#include <wchar.h>
	#include <stdio.h>
	#include <stdbool.h>
	// Debug mode
	void debug(wchar_t* a,...) {
		#ifndef DEBUG
		#else    
			va_list args;
			va_start(args, a);
			wprintf(a, args);
			va_end(args);
		#endif
	}
	#include "inc/list.h"
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
	BOOL Rshift, Ralt, Rctrl, Rwin, Lshift, Lalt, Lctrl, Lwin;
	//Main 2 layouts
	unsigned int LAYOUT1;
	unsigned int LAYOUT2;
	//If NMahou input.
	BOOL SELF = FALSE;
	// For JKL
	HWND MAIN_SERV;
	HMODULE lib;
	UINT uMSG; 
#endif