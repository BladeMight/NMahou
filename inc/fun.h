/* Contains all NMahou functions */
BOOL printable(int code) {
	BOOL numoff = (GetKeyState(VK_NUMLOCK) & 1) != 0;
	if ((code >= 0x30 && code <= 0x5A) ||
	    (code >= VK_OEM_1 && code <= VK_OEM_102) || 
		(numoff && (code >= VK_NUMPAD0 && code <= VK_NUMPAD9)) ||
		(code >= VK_MULTIPLY && code <= VK_DIVIDE))
		return TRUE;
	else
		return FALSE;
}
BOOL extended(int code) {
	switch(code) {
		case VK_MENU:    case VK_LMENU:    case VK_RMENU:
		case VK_CONTROL: case VK_RCONTROL: case VK_CANCEL:
		case VK_END: 	 case VK_INSERT:   case VK_DELETE:
		case VK_HOME:	 case VK_NEXT: 	   case VK_PRIOR:
		case VK_RIGHT:   case VK_LEFT:     case VK_DOWN:
		case VK_UP: 	 case VK_SNAPSHOT: case VK_NUMLOCK:
		case VK_DIVIDE:
			return TRUE;
		default:
			return FALSE;
	}
}
void WriteString(wchar_t* str) {
	int strlen = wcslen(str);
	INPUT* result = (INPUT*)malloc(strlen * sizeof(INPUT));
	for (int i = 0; i != strlen; i++) {
		INPUT ip;
		ip.type = INPUT_KEYBOARD;
		ip.ki.wVk = 0;
		ip.ki.dwFlags = KEYEVENTF_UNICODE;
		ip.ki.wScan = str[i]; 
		ip.ki.dwExtraInfo = 0;
		ip.ki.time = 0;
		if ((str[i] == L'\n' && str[i-1] != L'\r') || 
			(str[i] == L'\r' && str[i+1] != L'\n')) {
			ip.ki.wScan = 0;
			ip.ki.wVk = VK_RETURN;
		}
		if (str[i] == L'\r' && str[i+1] == L'\n') {
			ip.ki.wScan = 0;
			ip.ki.wVk = VK_RETURN;
			continue;
		}
        result[i] = ip;
        // debug(L"Added %d\n", str[i]);
	}
	SendInput(strlen, result, sizeof(INPUT));
	// free(result);
}
void HoldKey(int vk) {
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wVk = vk;
	ip.ki.dwFlags = extended(vk) ? KEYEVENTF_EXTENDEDKEY : 0;
	ip.ki.wScan = MapVirtualKey(vk, 0); 
	ip.ki.dwExtraInfo = 0;
	ip.ki.time = 0;
	SendInput(1, &ip, sizeof(INPUT));
}
void ReleaseKey(int vk) {
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wVk = vk;
	ip.ki.dwFlags = (extended(vk) ? KEYEVENTF_EXTENDEDKEY : 0) | KEYEVENTF_KEYUP;
	ip.ki.wScan = MapVirtualKey(vk, 0); 
	ip.ki.dwExtraInfo = 0;
	ip.ki.time = 0;
	SendInput(1, &ip, sizeof(INPUT));
}
void PressKey(int vk) {
	HoldKey(vk);
	ReleaseKey(vk);
}
void SetModifs(int code, BOOL value) {
	switch(code) {
		case VK_LSHIFT: 
			Lshift = value;	 break;
		case VK_RSHIFT:
			Rshift = value;	 break;
		case VK_LCONTROL:
			Lctrl = value; 	 break;
		case VK_RCONTROL:
			Rctrl = value; 	 break;
		case VK_LMENU: 
			Lalt = value; 	 break;
		case VK_RMENU:
			Ralt = value; 	 break;
		case VK_LWIN:
			Lwin = value; 	 break;
		case VK_RWIN:
			Rwin = value; 	 break;
	}
}
void SendModifiersUp() {
	if (Lshift)
		ReleaseKey(VK_LSHIFT);
	if (Lalt)
		ReleaseKey(VK_LMENU);
	if (Lctrl)
		ReleaseKey(VK_LCONTROL);
	if (Lwin)
		ReleaseKey(VK_LWIN);
	if (Rshift)
		ReleaseKey(VK_RSHIFT);
	if (Ralt)
		ReleaseKey(VK_RMENU);
	if (Rctrl)
		ReleaseKey(VK_RCONTROL);
	if (Rwin)
		ReleaseKey(VK_RWIN);
}
void SendClipCopy() {
	HoldKey(VK_RCONTROL);
	PressKey(VK_INSERT);
	ReleaseKey(VK_RCONTROL);
	Sleep(30);
}
void ChangeLayout() {
	// if (cyclemode)
	PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 0, HKL_NEXT);
	// else if (simulate)
	// else
}
void ConvertTyped(list_t* word) {
	SELF = TRUE;
	SendModifiersUp();
	ChangeLayout();
	for (int i = 0; i < word->lenght; i++)
		PressKey(VK_BACK);
	for (int i = 0; i <= word->lenght; i++) {
		int key = index_val(word, i);
		PressKey(key);
	}
	SELF = FALSE;
}
wchar_t* InAnotherLayout(wchar_t c, unsigned int layout1, unsigned int layout2) {
	unsigned int scan = VkKeyScanExW(c, (HKL)(uintptr_t)(layout1 & 0xffff));
	if (scan == -1) return L"";
	// debug(L"B %c, S %i, L %i, l %i\n", c, scan, (layout1 & 0xffff), (layout2 & 0xffff));
	int CST = (scan >> 8) & 0xff;
	BYTE* state = (BYTE*)malloc(256);
	if (CST == 1)
		state[VK_SHIFT] = 0xFF;
	wchar_t* character = (wchar_t*)malloc(sizeof(wchar_t) * 2); wcscpy(character, L"");
	ToUnicodeEx(scan, scan, state, character, 3, 0, (HKL)(uintptr_t)(layout2 & 0xffff));
	// debug(L"character is [%s]\n", character);
	// free(state);
	// free(character);
	return character;
}
wchar_t* wcsztok(wchar_t* str, const wchar_t* delim) {
    static wchar_t* static_str = 0;
    int index = 0, strlength = 0;
    int found = 0;	
    if (delim == 0 || (str == 0 && static_str == 0))
        return 0;	
    if (str == 0)
        str = static_str;	
    while(str[strlength])
        strlength++;	
    for (index = 0; index < strlength; index++)
        if (str[index] == delim[0]) {
            found=1;
            break;
        }
    if (!found) {
        static_str = 0;
        return str;
    }
    if (str[0] == delim[0]) {
        static_str = (str + 1);
        return (wchar_t* )delim;
    }
    str[index] = '\0';
    if ((str + index + 1)!=0)
        static_str = (str + index + 1);
    else
        static_str = 0;
	return str;
}
void ConvertSelection() {
	SELF = TRUE;
	LPWSTR backup = GetClipboardText();
	wchar_t* b = (wchar_t*)malloc((wcslen(backup)+1) * sizeof(wchar_t));
	wcscpy(b, L"");
	wcscpy(b, backup);
	debug(L"Clipboard backup: [%s]\n", backup);
	ClearClipboard();
	SendClipCopy();
	LPWSTR selection = GetClipboardText();
	if (selection != NULL && selection != L"") {
		int selen = wcslen(selection);
		wchar_t* result = (wchar_t*)malloc(sizeof(wchar_t) * (selen+1)); 
		wcscpy(result, L"");
		wchar_t* line;
		wchar_t* word;
		line = wcstok(selection, L"\n");
		debug(L"Selection is [%s](%i).", selection, selen);
		while (line != NULL) {
			debug (L"Line: [%s],\n  Words: ", line, selen);
			word = wcsztok(line, L" ");
			while (word != NULL) {
				int wolen = wcslen(word);
				debug(L"(%s)", word);
				int woL1min = 0;
				int woL2min = 0;
				wchar_t* wordL1 = (wchar_t*)malloc((wolen+1) * sizeof(wchar_t));
				wcscpy(wordL1, L"");
				wchar_t* wordL2 = (wchar_t*)malloc((wolen+1) * sizeof(wchar_t));
				wcscpy(wordL2, L"");
				for (int i = 0; i < wolen; i++) {
					debug(L"WOLEN: %i, I: %i, RLEN: %i\n", wolen, i, wcslen(result));
					LPWSTR CL1 = InAnotherLayout(word[i], LAYOUT2, LAYOUT1);
					LPWSTR CL2 = InAnotherLayout(word[i], LAYOUT1, LAYOUT2);
					if ((wcscmp(CL1, L"") == 0) && (wcscmp(CL2, L"") == 0) ||
						wcscmp(CL1, CL2) == 0) {
						debug(L"Rewriting [%c].\n", word[i]);
						wordL1[i] = word[i];
						wordL2[i] = word[i];
					} else {
						if (wcscmp(CL1, L"") != 0)
							wcscat(wordL1, CL1);
						else
							woL1min++;
						if (wcscmp(CL2, L"") != 0)
							wcscat(wordL2, CL2);
						else
							woL2min++;
					}
					debug(L"C1 [%s], C2 [%s]\n", CL1, CL2);
				}
				debug(L"W1 [%s], W2 [%s]\n", wordL1, wordL2);
				debug(L"cc1 [%i], cc2 [%i]\n", woL1min, woL2min);
				if (woL1min < woL2min)
					wcscat(result, wordL1);
				else
					wcscat(result, wordL2);
				word = wcsztok(NULL, L" ");
				if (word != NULL && word != L" ")
					wcscat(result, L" ");
				// free(wordL1);
				// free(wordL2);
			}
			debug(L"\n");
			line = wcstok(NULL, L"\n");
			if (line != NULL) // If not LAST line
				wcscat(result, L"\n");
		}
		debug(L"Whole selection [%s].\n", selection);
		debug(L"INPUTTING: result [%s]\n", result);
		WriteString(result);
		// free(result);
	}
	if (backup != NULL) {
		SetClipboardText(b);
		Sleep(30);
		// free(b);
	}
	SELF = FALSE;
}
int CheckHotkey(int code) {
	if (Lshift && code == VK_F7) {
		ConvertTyped(c_word);
		return 1;
	}
	if (code == VK_F7) {
		ConvertTyped(c_line);
		return 2;
	}
	if (code == VK_F9) {
		ConvertSelection();
		return 3;
	}
	if (code == VK_F6) {
		WriteString(L"Sword");
		print_list(c_line);
		print_list(c_word);
		print_list(a_layouts);
		return 4;
	}
	if (code == VK_F4) {
		PostQuitMessage(WM_QUIT);
		return 7;
	}
	if (code == VK_F11) {
		debug(L"%s\n", GetClipboardText());
		return 5;
	}
	if (code == VK_F2) {
		PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 0, HKL_NEXT);
		return 6;
	}
	if (code == VK_F12) {
		SetClipboardText(L"Copied to clipboard by NMahou. Just for testing. LOL...");
		return 7;
	}
	return -1;
}