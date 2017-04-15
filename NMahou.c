#include "inc/KMHooks.h"
#include "inc/WinHook.h"
#include "res/resource.h"
#include <io.h>
#include "NMahou.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow)
{
	//Allows displaying of Unicode characters in console.
	_setmode(_fileno(stdout), _O_U16TEXT);
	c_word = InitList(-1);
	MSG Msg;
	WNDCLASS wc = { 0 };
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	// wc.hInstance     = hInstance;
	wc.lpszClassName = "Main Window Class";
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	RegisterClass(&wc);
	HHOOK NMMHook = SetWindowsHookEx( WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance,0);
	HWND NMMWindow = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)WndProc, 0);
	NMMainHWND = NMMWindow;
	if(NMMWindow)
	{
		ShowWindow(NMMWindow, nCmdShow);
		UpdateWindow(NMMWindow);
		while (GetMessage(&Msg, 0, 0, 0) > 0)
		{
			if(!IsDialogMessage(NMMWindow, &Msg))
			{
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
		}
	}
	// MessageBoxW(NULL, L"ハロー！",L"フィールダー", 0);
	// wprintf(L"\nWindows loop ended, starting console loop...\n");
	// while (GetMessage(&Msg, FALSE, FALSE, FALSE) > 0) {    //this while loop keeps the hook
        // TranslateMessage(&Msg);
        // DispatchMessage(&Msg);
    // }
	return(0);
}
