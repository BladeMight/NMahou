#include <windows.h>
#include <wchar.h>
#include <stdio.h>
#include <stdbool.h>
#include "../NMahou.h"
#include "clipboard.h"
#include "fun.h"

//The function that implements the key logging functionality
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	// Declare a pointer to the KBDLLHOOKSTRUCT
	KBDLLHOOKSTRUCT *kbdact = (KBDLLHOOKSTRUCT *)lParam;
	// Initialize variables
	unsigned int code = kbdact->vkCode;
	LONG scan = kbdact->scanCode << 16;
	if (GetForegroundWindow() != NMMainHWND) {
		wchar_t* lpszName = malloc(256); wcscpy(lpszName, L"");
		GetKeyNameTextW(scan, lpszName, 256);
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
			SetModifs(code, true);
		else if (wParam == WM_KEYUP)
			SetModifs(code, false);
		wchar_t* modifs = malloc(16); wcscpy(modifs, L"");
		shift ?	wcscat(modifs, L"Shift") : wcscat(modifs, L"");
		if (alt)
			wcscat(modifs, L"Alt");
		if (ctrl)
			wcscat(modifs, L"Ctrl");
		if (win)
			wcscat(modifs, L"Win");
		//wprintf(L"Sh=%i, Al=%i, Ct=%i, Wi=%i, wParam=%i.\n", shift, alt, ctrl, win, wParam);
		BYTE kst[256];
		if (shift)
			kst[16] = 0xff;
		else
			kst[16] = 0x0;
		if (code == 16 || code == 161 || code == 160) {
			if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
				shift = true;
			else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
				shift = false;
		}
		LPWSTR ch = malloc(sizeof(LPWSTR));
		if (printable(code))
			ToUnicodeEx(code,0,kst,ch,3,0,GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), NULL)));
			//wprintf(L"ToUni returned =[%i], parameters c=[%i],s=[%i],k=[%i],ch=[%i],l=[%i].\n",,code, 0, kst, ch, GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), NULL)));
		else
			ch = L"";
		if (!SELF) 
		switch(wParam) {
			case WM_KEYUP:
			case WM_SYSKEYUP:
					wprintf(L"Released key with VK_CODE=[%i], KEY_NAME=[%s]", code, lpszName);
					if (printable(code))
						wprintf(L", KEY_CHARACTER = [%s]", ch);
					if (modifs != L"")
						wprintf(L", modifiers=[%s]", modifs);
					wprintf(L".\n");
					if (code == VK_RMENU) {
						wprintf(L"LOL:\n");
						keybd_event(VK_LMENU, MapVirtualKey(VK_LMENU, 0), KEYEVENTF_EXTENDEDKEY | 0, 0);
						keybd_event(VK_LSHIFT, MapVirtualKey(VK_LSHIFT, 0), 0, 0);
						keybd_event(VK_LSHIFT, MapVirtualKey(VK_LSHIFT, 0), KEYEVENTF_KEYUP, 0);
						keybd_event(VK_LMENU, MapVirtualKey(VK_LMENU, 0), KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
					}
					break;
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
					wprintf(L"Pressed key with VK_CODE=[%i], KEY_NAME=[%s], ", code, lpszName);
					if (printable(code))
						wprintf(L", KEY_CHARACTER = [%s]", ch);
					if (modifs != L"")
						wprintf(L", modifiers=[%s]", modifs);
					wprintf(L".\n");
					if (code == VK_BACK)
						rem(c_word);
					else if (printable(code))
						add(c_word, code, ch);
					if (code == VK_RETURN || code == VK_HOME || code == VK_END ||
					   code == VK_TAB|| code == VK_PRIOR|| code == VK_NEXT ||
					   code == VK_LEFT || code == VK_RIGHT || code == VK_DOWN || code == VK_UP ||
					   code == VK_BROWSER_SEARCH || 
					   (ctrl && code != 0))
					   clear(&c_word);
					if (!SELF && code == VK_F7)
						ConvertLastWord();
					if (code == VK_F11)
						wprintf(L"%s\n", GetClipboardText());
					if (code == VK_F12)
						SetClipboardText(L"LOLRRRRRRRRRRRRRRRRRrr IT WORKS");
					if (code == VK_F6) {
						print_list(c_word);
						print_list(a_layouts);
					}
					if (code == VK_F4)
						PostQuitMessage(WM_QUIT);
					if (code == VK_F2)
						PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 0, HKL_NEXT);
					if (code == VK_F8)
						PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 0, 1041);
					if (code == VK_F9)
						ConvertSelection();
					break;
		}
	}
	return CallNextHookEx( NULL, nCode, wParam, lParam);
}
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		if (wParam == WM_RBUTTONDOWN || wParam == WM_LBUTTONDOWN) {
			clear(&c_word);
			wprintf(L"Cleared by mouse click.\n");
		}
	}
	return CallNextHookEx( NULL, nCode, wParam, lParam);
}
