#include <windows.h>

BOOL OpenClip() {
  if (!OpenClipboard(NULL)) {
	debug(L"Clipboard can't be opened, error: %i.\n", GetLastError());
	return FALSE;
  }
  return TRUE;
}

void ClearClipboard() {	
	if (!OpenClip()) return;
	EmptyClipboard();
	CloseClipboard();
}
wchar_t* GetClipboardText() {
  if (!OpenClip()) return L"";
  HANDLE hData = GetClipboardData(CF_UNICODETEXT);
  if (hData == NULL) {
	debug(L"Clipboard has no data.\n");
	return L"";
  }
  wchar_t* pszText = (wchar_t*)GlobalLock(hData);
  if (pszText == NULL) {
	debug(L"Clipboard data handle lock has no data.\n");
	return L"";
  }
  GlobalUnlock(hData);
  CloseClipboard();
  return pszText;
}

void SetClipboardText(wchar_t* _text) {
	if(OpenClip()) {
		HGLOBAL clipbuffer;
		wchar_t* buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, sizeof(wchar_t) * (wcslen(_text)+1));
		buffer = (wchar_t*)GlobalLock(clipbuffer);
		wcscpy(buffer, _text);
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_UNICODETEXT, clipbuffer);
		CloseClipboard();
	}
}