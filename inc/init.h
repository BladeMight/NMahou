#include <wchar.h>
#include <windows.h>
#include "../jkl/jkl.h"
void InitVersion() { // Initializes Native Mahou version string
	NM_VERSION = malloc(6 * sizeof(wchar_t)); wcscpy(NM_VERSION, L"0.023");
}
void InitTrayIcon() {
	// Main window title 
	LPWSTR Title = malloc(256 * sizeof(wchar_t)); wcscpy(Title, L"");
	wcscat(Title, L"Native Mahou "); wcscat(Title, NM_VERSION);
	SetWindowText(NMMainHWND, Title);
	// Creating tray icon
	NMTrayIcon.cbSize = sizeof(NOTIFYICONDATA);
	NMTrayIcon.hWnd = NMMainHWND;
	NMTrayIcon.uID = IDI_NMICON;
	NMTrayIcon.uVersion = NOTIFYICON_VERSION;
	NMTrayIcon.uCallbackMessage = WM_SYSICON;
	NMTrayIcon.hIcon = LoadIcon(NMInstance, MAKEINTRESOURCE(IDI_NMICON));
	NMTrayIcon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	wcscpy(NMTrayIcon.szTip, Title);
	// Converting Title to ascii
	// char* ascii = malloc(sizeof(WCHAR) * wcslen(Title)); strcpy(ascii, "");
	// wcstombs(ascii, Title, wcslen(Title) );
	// strcpy(NMTrayIcon.szTip, ascii);
	wprintf(L"Creating tray menu...\n");
	HMENU hn = LoadMenu(NULL, MAKEINTRESOURCE(IDM_TRAYMENU));
	NMTrayMenu = GetSubMenu(hn, 0);
	// Placing tray icon in *tray*
	Shell_NotifyIcon(NIM_ADD, &NMTrayIcon);
}
void InitJKL() {
	MAIN_SERV = CreateHWND(NMInstance, L"_HIDDEN_HWND_SERVER", (WNDPROC)ServerProc);
	if (FileExist(L"jkl.dll")) {
		lib = LoadLibrary(L"jkl.dll");
		FUNC setHook = (FUNC)GetProcAddress(lib, "setHook");
		if (!setHook())
			wprintf(L"HOOK SET FAILED!");
		UFUNC getUMsg = (UFUNC)GetProcAddress(lib, "getUMsg");
		uMSG = getUMsg();
		#ifndef SUPPORTX86
			if (FileExist(L"jklx86.exe"))
				ShellExecute(0, L"open", L"jklx86.exe", NULL, L".\\", SW_HIDE);
			else
				wprintf(L"x86 support won't be available...\n");
		#endif
	} else 
		wprintf(L"jkl.dll not found...\n");
}