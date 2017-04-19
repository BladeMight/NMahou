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
	//Main Handle of Main Window
	static HWND NMMainHWND;
	//Main list of keys
	list_t* c_word;
	//List of all layouts
	list_t* a_layouts;
	//Modifiers
	bool shift, alt, ctrl, win;
	//Main 2 layouts
	unsigned int LAYOUT1;
	unsigned int LAYOUT2;
	//If NMahou input.
	bool SELF = false;
#endif