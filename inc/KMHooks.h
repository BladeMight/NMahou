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
		wchar_t* keyName = malloc(256); wcscpy(keyName, L"");
		GetKeyNameTextW(scan, keyName, 256);
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
			SetModifs(code, true);
		else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
			SetModifs(code, false);
		wchar_t* modifs = malloc(58); wcscpy(modifs, L"");
		if (Lshift)
			wcscat(modifs, L"L-Shift ");
		if (Lalt)
			wcscat(modifs, L"L-Alt ");
		if (Lctrl)
			wcscat(modifs, L"L-Ctrl ");
		if (Lwin)
			wcscat(modifs, L"L-Win ");
		if (Rshift)
			wcscat(modifs, L"R-Shift ");
		if (Ralt)
			wcscat(modifs, L"R-Alt ");
		if (Rctrl)
			wcscat(modifs, L"R-Ctrl ");
		if (Rwin)
			wcscat(modifs, L"R-Win ");
		BYTE* kst = malloc(256);
		if (Lshift || Rshift)
			kst[16] = 0xFF;
		LPWSTR ch = malloc(sizeof(LPWSTR));
		if (printable(code))
			ToUnicodeEx(code,0,kst,ch,3,0,GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), NULL)));
		else
			ch = L"";
		if (!SELF) 
		switch(wParam) {
			case WM_KEYUP:
			case WM_SYSKEYUP:
					wprintf(L"Released key with VK_CODE=[%i], KEY_NAME=[%s]", code, keyName);
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
					wprintf(L"Pressed key with VK_CODE=[%i], KEY_NAME=[%s], ", code, keyName);
					if (printable(code))
						wprintf(L", KEY_CHARACTER = [%s]", ch);
					if (modifs != L"")
						wprintf(L", modifiers=[%s]", modifs);
					wprintf(L".\n");
					if (code == VK_BACK)
						rem(c_word);
					else if (printable(code)) {
						add(c_word, code, ch);
						add(c_line, code, ch);
					} else if (code == VK_SPACE) {
						add(c_line, code, ch);
						clear(&c_word);
					}
					if (code == VK_RETURN || code == VK_HOME || code == VK_END ||
					   code == VK_TAB|| code == VK_PRIOR|| code == VK_NEXT ||
					   code == VK_LEFT || code == VK_RIGHT || code == VK_DOWN || code == VK_UP ||
					   code == VK_BROWSER_SEARCH ||
					   (Lctrl && code != 0)) {
					   clear(&c_word);
					   clear(&c_line);
					}
					wprintf(L"Hotkey <%i> fired(-1 = not).\n", CheckHotkey(code));
					break;
		}
	}
	return CallNextHookEx( NULL, nCode, wParam, lParam);
}
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		if (wParam == WM_RBUTTONDOWN || wParam == WM_LBUTTONDOWN) {
			clear(&c_word);
			clear(&c_line);
			wprintf(L"Cleared by mouse click.\n");
		}
	}
	return CallNextHookEx( NULL, nCode, wParam, lParam);
}
