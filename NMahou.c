#include "inc/KMHooks.h"
#include "inc/WinHook.h"
#include "res/resource.h"
#include <io.h>
#include "NMahou.h"
#include "inc/init.h"

void RefreshLayouts() {
	UINT LayoutsCount = GetKeyboardLayoutList(0, NULL);
	HKL* layouts = (HKL*)LocalAlloc(LPTR, (LayoutsCount * sizeof(HKL)));
	GetKeyboardLayoutList(LayoutsCount, layouts);
	for (int i = 0; i < LayoutsCount; i++) {
		// debug(L"%i\n", layouts[i]);
		HKL l = layouts[i];
		add(a_layouts, (uintptr_t)l, L"");
		LAYOUT2 = (uintptr_t)l;
	}
	a_layouts->lenght = LayoutsCount - 1;
	LAYOUT1 = index_val(a_layouts, 0);
	print_list(a_layouts);
}
void InitializeMain() {
	InitVersion();
	InitTrayIcon();
	InitJKL();
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow) {
	//Allows displaying of Unicode characters in console.
	NMInstance = hInstance;
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);
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
