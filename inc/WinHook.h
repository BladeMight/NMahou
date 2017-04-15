#include <windows.h>
#include <stdio.h>
#include "../res/resource.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	int Result = 0;
	switch (Msg) {
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case 9:
			case 3:
				wprintf(L"Button [%i] pressed!\n", LOWORD(wParam));
				break;
			// case 666:
				// wprintf(L"Combobox index changed.\n");
				// HWND cmbox = GetDlgItem(hWnd, 666);
				// int cunya = SendMessage(cmbox,CB_GETCURSEL, (WORD)0, 0L);
				// char* value[32];
				// SendMessageW(cmbox, CB_GETLBTEXT, (WPARAM)cunya, (LPARAM)value);
				// wprintf(L"Selected item index=[%i], value=[%s].\n", cunya, value);
				// break;
			case 7:
				wprintf(L"Configurations changed.\n");
				break;
			default:
				wprintf(L"Some command launched, hwnd=[%i] && id=[%i]!\n", lParam, LOWORD(wParam));
				break;
			}
			break;
		case WM_DESTROY:
			wprintf(L"Shutdown passed...");
			PostQuitMessage(0);
			break;
		default:
			Result = DefWindowProc(hWnd, Msg, wParam, lParam);
			break;
	}
	return (Result);
}
