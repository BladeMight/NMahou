/* Contains all NMahou functions */
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
void SendClipCopy() {
	keybd_event(VK_RCONTROL, MapVirtualKey(VK_RCONTROL, 0), KEYEVENTF_EXTENDEDKEY | 0, 0);
	keybd_event(VK_INSERT, MapVirtualKey(VK_INSERT, 0), KEYEVENTF_EXTENDEDKEY | 0, 0);
	keybd_event(VK_INSERT, MapVirtualKey(VK_INSERT, 0), KEYEVENTF_EXTENDEDKEY | 2, 0);
	keybd_event(VK_RCONTROL, MapVirtualKey(VK_RCONTROL, 0), KEYEVENTF_EXTENDEDKEY | 2, 0);
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
LPWSTR InAnotherLayout(wchar_t c, unsigned int layout1, unsigned int layout2) {
	unsigned int scan = VkKeyScanExW(c, (HKL)(uintptr_t)(layout1 & 0xffff));
	if (scan == -1) return L"";
	wprintf(L"B %c, S %i, L %i, l %i\n", c, scan, (layout1 & 0xffff), (layout2 & 0xffff));
	int CST = (scan >> 8) & 0xff;
	BYTE* state = malloc(256);
	if (CST == 1)
		state[VK_SHIFT] = 0xFF;
	LPWSTR character = malloc(sizeof(LPWSTR));
	ToUnicodeEx(scan, scan, state, character, 3, 0, (HKL)(uintptr_t)(layout2 & 0xffff));
	wprintf(L"character is [%s]\n", character);
	return character;
}
void InputString(wchar_t* str) {
	for (int i = 0; i <= wcslen(str); i++) {
		wchar_t s = str[i];
		INPUT down;
		down.type = INPUT_KEYBOARD;
		down.ki.wVk = 0;
		down.ki.wScan = s;
		down.ki.time = 0;
		down.ki.dwExtraInfo = 0;
		down.ki.dwFlags = KEYEVENTF_UNICODE;
		INPUT up;
		up.type = INPUT_KEYBOARD;
		up.ki.wVk = 0;
		up.ki.wScan = s;
		up.ki.time = 0;
		up.ki.dwExtraInfo = 0;
		up.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
		if ((s == L'\n' && str[i-1] != L'\r') || 
			(s == L'\r' && str[i+1] != L'\n')) {
			down.ki.wScan = 0;
			down.ki.wVk = VK_RETURN;
			up.ki.wScan = 0;
			up.ki.wVk = VK_RETURN;
		}
		SendInput(1, &down, sizeof(INPUT));
		SendInput(1, &up, sizeof(INPUT));
	}
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
	SELF = true;
	LPWSTR backup = GetClipboardText();
	ClearClipboard();
	SendClipCopy();
	LPWSTR selection = GetClipboardText();
	if (selection != NULL && selection != L"") {
		unsigned int selen = wcslen(selection);
		wchar_t* result = malloc(selen+1); wcscpy(result, L"");
		wchar_t* line;
		wchar_t* word;
		// wchar_t* nu;
		line = wcstok(selection, L"\n");
		while (line != NULL) {
			wprintf (L"Line: [%s],\n  Words: ", line);
			word = wcsztok(line, L" ");
			while (word != NULL) {
				int wolen = wcslen(word);
				wprintf(L"(%s)", word);
				// wprintf(L"\n{%s}\n", nu);
				int woL1min = 0;
				int woL2min = 0;
				wchar_t* wordL1 = malloc(wolen+1); wcscpy(wordL1, L"");
				wchar_t* wordL2 = malloc(wolen+1); wcscpy(wordL2, L"");
				for (int i = 0; i < wolen; i++) {
					wprintf(L"WOLEN: %i, I: %i, RLEN: %i\n", wolen, i, wcslen(result));
					LPWSTR CL1 = InAnotherLayout(word[i], LAYOUT2, LAYOUT1);
					LPWSTR CL2 = InAnotherLayout(word[i], LAYOUT1, LAYOUT2);
					if ((wcscmp(CL1, L"") == 0) && (wcscmp(CL2, L"") == 0)) {
						wprintf(L"Rewriting [%c].\n", word[i]);
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
					wprintf(L"W1 [%s], W2 [%s]\n", wordL1, wordL2);
					wprintf(L"C1 [%s], C2 [%s]\n", CL1, CL2);
					wprintf(L"cc1 [%i], cc2 [%i]\n", woL1min, woL2min);
				}
				if (woL1min < woL2min)
					wcscat(result, wordL1);
				else
					wcscat(result, wordL2);
				word = wcsztok(NULL, L" ");
				if (word != NULL && word != L" ")
					wcscat(result, L" ");
			}
			wprintf(L"\n");
			line = wcstok(NULL, L"\n");
			if (line != NULL) // If not LAST line
				wcscat(result, L"\n");
		}
		wprintf(L"Whole selection [%s].\n", selection);
		wprintf(L"INPUTTING: result [%s]\n", result);
		InputString(result);
	}
	if (backup != NULL)
		SetClipboardText(backup);
	SELF = false;
}