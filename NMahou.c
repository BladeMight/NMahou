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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow) {
	//Allows displaying of Unicode characters in console.
	_setmode(_fileno(stdout), _O_U16TEXT);
	c_word = InitList(-1, L"");
	c_line = InitList(-1, L"");
	a_layouts = InitList(-1, L"");
	RefreshLayouts();
	MSG Msg;
	HHOOK NMMKHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance,0);
	HHOOK NMMMHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInstance,0);
	HWND NMMWindow = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)WndProc, 0);
	NMMainHWND = NMMWindow;
	SetWindowTextW(NMMainHWND, L"Native Mahou 0.02");
	if(NMMWindow) {
		ShowWindow(NMMWindow, nCmdShow);
		UpdateWindow(NMMWindow);
		while (GetMessage(&Msg, 0, 0, 0) > 0) {
			if(!IsDialogMessage(NMMWindow, &Msg)) {
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
		}
	}
	return(0);
}
