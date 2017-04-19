#include <windows.h>

void ClearClipboard() {	
	OpenClipboard(NULL);
	EmptyClipboard();
	CloseClipboard();
}
LPWSTR GetClipboardText() {
	OpenClipboard(NMMainHWND);
	LPWSTR _GetText = GetClipboardData(CF_UNICODETEXT);
	for (int i = 0; i != 3; i++) {
		wprintf(L"Tries %i, result: %s.\n", i, _GetText);
		if (_GetText == NULL)
			_GetText = GetClipboardData(CF_UNICODETEXT);
		else break;
	}
	CloseClipboard();
	return _GetText;
}

void SetClipboardText(LPWSTR _Text) {
	LPWSTR _TextBuffer;
	HGLOBAL _SetBuffer;
	wprintf(L"Settings [%s] text to clipboard...", _Text);
	_SetBuffer = GlobalAlloc(GHND, (wcslen(_Text) + 1) * sizeof(wchar_t)); 
	if(_SetBuffer != NULL) {
		_TextBuffer = (LPWSTR)GlobalLock(_SetBuffer);
		if(_TextBuffer != NULL) {
			wcscpy(_TextBuffer, _Text);
			GlobalUnlock(_SetBuffer);
			OpenClipboard(NULL);
			EmptyClipboard();
			SetClipboardData(CF_UNICODETEXT, _SetBuffer);
			GlobalUnlock(_TextBuffer);
			CloseClipboard();
		}
	}
}