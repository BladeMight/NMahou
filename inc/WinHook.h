#include <windows.h>
#include <stdio.h>
#include "../res/resource.h"
void ToggleVisibility() {	
	if (!IsWindowVisible(NMMainHWND)) {
		ShowWindow(NMMainHWND, SW_SHOW);
		SetForegroundWindow(NMMainHWND);
	}
	else 
		ShowWindow(NMMainHWND, SW_HIDE);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	int Result = 0;
	switch (Msg) {
		case WM_SYSICON: {
			if (lParam == WM_LBUTTONUP) {
				wprintf(L"Tray clicked w/ LMB\n");
				ToggleVisibility();
			}
			else if (lParam == WM_RBUTTONDOWN)  {
				wprintf(L"Tray clicked w/ RMB\n");
				// Get current mouse position.
				POINT curPoint;
				GetCursorPos(&curPoint);

				// TrackPopupMenu blocks the app until TrackPopupMenu returns

				UINT clicked = TrackPopupMenu(NMTrayMenu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, hWnd, NULL);


				
				SendMessage(hWnd, WM_NULL, 0, 0); // send benign message to window to make sure the menu goes away.
				if (clicked == IDM_EXIT) {
					PostMessage(NMMainHWND, WM_DESTROY, wParam, lParam);
				} else if (clicked == IDM_TOGGLE) 
					ToggleVisibility(); 
			}
		}
		break;
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
			Shell_NotifyIcon(NIM_DELETE, &NMTrayIcon);
			PostQuitMessage(0) ;
			break;
		default:
			Result = DefWindowProc(hWnd, Msg, wParam, lParam);
			break;
	}
	return (Result);
}
