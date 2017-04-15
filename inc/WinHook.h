#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "../res/resource.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// HINSTANCE hIns;
	int Result = 0;
	HFONT font = CreateFontW(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Segoe UI");
	HWND hCtl;
	switch (Msg)
	{
	case WM_CREATE:
		SendMessage(hCtl, WM_SETFONT, (WPARAM)font, 0);
		break;
	case WM_CTLCOLORSTATIC:
		SetTextColor((HDC)wParam, RGB(109, 194, 222));
		SetBkMode((HDC)wParam, TRANSPARENT);
		return GetClassLong(hWnd, GCL_HBRBACKGROUND);
	case WM_CTLCOLOREDIT:
		SetTextColor((HDC)wParam, RGB(255, 0, 255));
		SetBkMode((HDC)wParam, TRANSPARENT);
		return GetClassLong(hWnd, GCL_HBRBACKGROUND);
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 777:
			wprintf(L"Button pressed!\n");
			break;
		case 666:
			wprintf(L"Combobox index changed.\n");
			HWND cmbox = GetDlgItem(hWnd, 666);
			int cunya = SendMessage(cmbox,CB_GETCURSEL, (WORD)0, 0L);
			char* value[32];
			SendMessageW(cmbox, CB_GETLBTEXT, (WPARAM)cunya, (LPARAM)value);
			wprintf(L"Selected item index=[%i], value=[%s].\n", cunya, value);
			break;
		case 888:
			{
				// wprintf(L"Checkbox state changed.\n");
				// wprintf(L"Some command launched, hwnd=[%i] && id=[%i], hi=[%i]!\n", lParam, LOWORD(wParam), HIWORD(wParam));
				switch (HIWORD(wParam))
				{
				case BN_CLICKED:
					if (SendDlgItemMessage(hWnd, 888, BM_GETCHECK, 0, 0))
					wprintf(L"Checkbox Selected.\n");
					else
					wprintf(L"Checkbox Deselected.\n");
					break;
				}
			}break;
		default:
			wprintf(L"Some command launched, hwnd=[%i] && id=[%i]!\n", lParam, LOWORD(wParam));
			break;
		}
		break;
	case WM_DESTROY:
		wprintf(L"Shutdown passed...");
		DeleteObject((HBRUSH)(INT_PTR)GetClassLong(hWnd, GCL_HBRBACKGROUND));
		PostQuitMessage(0);
		break;
	default:
		Result = DefWindowProc(hWnd, Msg, wParam, lParam);
		break;
	}

	return (Result);
}
