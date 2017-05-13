#include "inc/KMHooks.h"
#include "inc/WinHook.h"
#include "res/resource.h"
#include <io.h>
#include "NMahou.h"

void RefreshLayouts() {
	UINT LayoutsCount = GetKeyboardLayoutList(0, NULL);
	HKL* layouts = (HKL*)LocalAlloc(LPTR, (LayoutsCount * sizeof(HKL)));
	GetKeyboardLayoutList(LayoutsCount, layouts);
	for (int i = 0; i < LayoutsCount; i++) {
		// wprintf(L"%i\n", layouts[i]);
		HKL l = layouts[i];
		add(a_layouts, (uintptr_t)l, L"");
		LAYOUT2 = (uintptr_t)l;
	}
	a_layouts->lenght = LayoutsCount - 1;
	LAYOUT1 = index_val(a_layouts, 0);
	print_list(a_layouts);
}
void InitializeMain() {
	// Main window title 
	LPWSTR Title = malloc(256 * sizeof(wchar_t)); wcscpy(Title, L"");
	wcscat(Title, L"Native Mahou "); wcscat(Title, NM_VERSION);
	SetWindowTextW(NMMainHWND, Title);
	// Creating tray icon
	NMTrayIcon.cbSize = sizeof(NOTIFYICONDATA);
	NMTrayIcon.hWnd = NMMainHWND;
	NMTrayIcon.uID = IDI_NMICON;
	NMTrayIcon.uVersion = NOTIFYICON_VERSION;
	NMTrayIcon.uCallbackMessage = WM_SYSICON;
	NMTrayIcon.hIcon = LoadIcon(NMInstance, MAKEINTRESOURCE(IDI_NMICON));
	// Converting Title to ascii
	char* ascii = malloc(sizeof(char) * wcslen(Title)); strcpy(ascii, "");
	wcstombs(ascii, Title, wcslen(Title) );
	strcpy(NMTrayIcon.szTip, ascii);
	NMTrayIcon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	wprintf(L"Creating tray menu...");
	HMENU hn = LoadMenu(NULL, MAKEINTRESOURCE(IDM_TRAYMENU));
	NMTrayMenu = GetSubMenu(hn, 0);
	// Placing tray icon in *tray*
	Shell_NotifyIcon(NIM_ADD, &NMTrayIcon);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow) {
	//Allows displaying of Unicode characters in console.
	NMInstance = hInstance;
	_setmode(_fileno(stdout), _O_U16TEXT);
	InitVersion();
	c_word = InitList(-1, L"");
	c_line = InitList(-1, L"");
	a_layouts = InitList(-1, L"");
	RefreshLayouts();
	MSG Msg;
	HHOOK NMMKHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NMInstance,0);
	HHOOK NMMMHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NMInstance,0);
	HWND NMMWindow = CreateDialogParam(NMInstance, MAKEINTRESOURCE(IDD_MMAIN), 0, (DLGPROC)WndProc, 0);
	NMMainHWND = NMMWindow;
	InitializeMain();
	if(NMMWindow) {
		while (GetMessage(&Msg, 0, 0, 0) > 0) {
			if(!IsDialogMessage(NMMWindow, &Msg)) {
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
		}
	}
	return(0);
}
