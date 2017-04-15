#include <windows.h>

LPWSTR GetClipboardText() {
	OpenClipboard(NMMainHWND);
	LPWSTR _GetText = GetClipboardData(CF_UNICODETEXT);
	for (int i = 0; i != 3; i++) {
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
	_SetBuffer = GlobalAlloc(GHND, wcslen(_Text) * sizeof(WCHAR)); 
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