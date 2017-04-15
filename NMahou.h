#ifndef _NMMHeader // NATIVE MAHOU MAIN HEADER
	#define _NMMHeader
	#include "inc/list.h"
	#define _O_U16TEXT 0x20000

	//Main Handle of Main Window
	static HWND NMMainHWND;
	//Main list of keys
	list_t* c_word;
	bool SELF = false;
#endif