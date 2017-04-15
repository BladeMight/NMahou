#include <windows.h>
#include <wchar.h>
#include <stdio.h>
#include <stdbool.h>
#include "../NMahou.h"

bool shift, alt, ctrl, win;

void SetModifs(int code, bool value) {
		switch(code) {
		case 160: case 161:
			shift = value;	 break;
		case 162: case 163:
			ctrl = value; 	 break;
		case 164: case 165:
			alt = value; 	 break;
		case 91: case 92:
			win = value; 	 break;
		}
}
bool printable(int code) {
	bool numoff = (GetKeyState(VK_NUMLOCK) & 1) != 0;
	if ((code >= 0x30 && code <= 0x5A) ||
	    (code >= VK_OEM_1 && code <= VK_OEM_102) || 
		(numoff && (code >= VK_NUMPAD0 && code <= VK_NUMPAD9)) ||
		(code >= VK_MULTIPLY && code <= VK_DIVIDE))
		return true;
	else
		return false;
}
bool extended(int code) {
	switch(code) {
		case VK_MENU:    case VK_LMENU:    case VK_RMENU:
		case VK_CONTROL: case VK_RCONTROL: case VK_CANCEL:
		case VK_END: 	 case VK_INSERT:   case VK_DELETE:
		case VK_HOME:	 case VK_NEXT: 	   case VK_PRIOR:
		case VK_RIGHT:   case VK_LEFT:     case VK_DOWN:
		case VK_UP: 	 case VK_SNAPSHOT: case VK_NUMLOCK:
		case VK_DIVIDE:
			return true;
		default:
			return false;
	}
}
void KeyPress(int key) {
	bool ext = extended(key);
	keybd_event(key, MapVirtualKey(key, 0), ext ? KEYEVENTF_EXTENDEDKEY : 0 | 0, 0);
	keybd_event(key, MapVirtualKey(key, 0), ext ? KEYEVENTF_EXTENDEDKEY : 0 | 2, 0);
}
void ChangeLayout() {
	// if (cyclemode)
	PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 0, HKL_NEXT);
	// else if (simulate)
	// else
}
void ConvertLastWord() {
	SELF = true;
	ChangeLayout();
	for (int i = 0; i < c_word->lenght; i++)
		KeyPress(VK_BACK);
	for (int i = 0; i <= c_word->lenght; i++) {
		int key = index_val(c_word, i);
		KeyPress(key);
	}
	SELF = false;
}
//The function that implements the key logging functionality
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	// Declare a pointer to the KBDLLHOOKSTRUCT
	KBDLLHOOKSTRUCT *kbdact = (KBDLLHOOKSTRUCT *)lParam;
	// Initialize variables
	unsigned int code = kbdact->vkCode;
	LONG scan = kbdact->scanCode << 16;
	if (GetForegroundWindow() != NMMainHWND) {
		WCHAR lpszName[256];
		GetKeyNameTextW(scan, lpszName, 256);
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
			SetModifs(code, true);
		else if (wParam == WM_KEYUP)
			SetModifs(code, false);
		wchar_t *modifs = malloc(5+3+3+3 * sizeof(wchar_t));
		shift ?	wcscpy(modifs, L"Shift") : wcscpy(modifs, L"");
		if (alt)
			wcscat(modifs, L"Alt");
		if (ctrl)
			wcscat(modifs, L"Ctrl");
		if (win)
			wcscat(modifs, L"Win");
		// printf("Sh=%i, Al=%i, Ct=%i, Wi=%i, wParam=%i.\n", shift, alt, ctrl, win, wParam);
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
			// wprintf(L"ToUni returned =[%i], parameters c=[%i],s=[%i],k=[%i],ch=[%i],l=[%i].\n",,code, 0, kst, ch, GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), NULL)));
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
						add(c_word, code);
					if (code == VK_RETURN || code == VK_HOME || code == VK_END ||
					   code == VK_TAB|| code == VK_PRIOR|| code == VK_NEXT ||
					   code == VK_LEFT || code == VK_RIGHT || code == VK_DOWN || code == VK_UP ||
					   code == VK_BROWSER_SEARCH || 
					   (ctrl && code != 0))
					   clear(&c_word);
					if (code == VK_F7)
						ConvertLastWord();
					if (code == VK_F6)
						print_list(c_word);
					if (code == VK_F4)
						PostQuitMessage(WM_QUIT);
					if (code == VK_F2)
						PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 0, HKL_NEXT);
					if (code == VK_F8)
						PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 0, 1041);
					if (code == VK_F9)
						PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 0, 1049);
					break;
		}
	}
	return CallNextHookEx( NULL, nCode, wParam, lParam);
}
