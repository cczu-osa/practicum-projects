// Front end for Hotel.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Front end for Hotel.h"

#define MAX_LOADSTRING 100


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text
HWND ghWnd;

int current_floor = 0;
int room_off = 0;

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	Order(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FRONTENDFORHOTEL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_FRONTENDFORHOTEL);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_FRONTENDFORHOTEL);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_FRONTENDFORHOTEL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   ghWnd = hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 700, 350, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void GetWindowPos(HWND hWnd, int *x, int *y)
{
    HWND hWndParent = GetParent(hWnd);
    POINT p = {0};

    MapWindowPoints(hWnd, hWndParent, &p, 1);

    (*x) = p.x;
    (*y) = p.y;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, x, y, i, wx, wy;
	TCHAR szHello[MAX_LOADSTRING];
	HBITMAP hBitmap;
	POINT cursorPos;
	CHAR  msg[128];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_CREATE:
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT 	ps;
    			HDC 			hdc;
    			BITMAP 			bitmap;
    			HDC 			hdcMem;
				HGDIOBJ 		oldBitmap;
				RECT            rt;
				
				int				i, x, y;

				hBitmap = (HBITMAP)LoadImage(hInst, "floor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    			hdc = BeginPaint(hWnd, &ps);

    			hdcMem = CreateCompatibleDC(hdc);
				oldBitmap = SelectObject(hdcMem, hBitmap);

				GetObject(hBitmap, sizeof(bitmap), &bitmap);
				for (i = 0; i != 26; ++i) {
					for (x = floors[current_floor][i].position.left; x != floors[current_floor][i].position.right; ++x)
						for (y = floors[current_floor][i].position.top; y != floors[current_floor][i].position.bottom; ++y) {
							COLORREF color = GetPixel(hdcMem, x, y);
							if (floors[current_floor][i].status == AVAILABLE) {
								color = PALETTERGB(GetRValue(color) >> 1, GetGValue(color), GetBValue(color) >> 1);
								SetPixel(hdcMem, x, y, color);
							}
							else {
								color = PALETTERGB(GetRValue(color), GetGValue(color) >> 1, GetBValue(color) >> 1);
								SetPixel(hdcMem, x, y, color);
							}
						}
				}
				BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
	
				SelectObject(hdcMem, oldBitmap);
				DeleteDC(hdcMem);
				DeleteObject(hBitmap);

				GetClientRect(hWnd, &rt);
				rt.top += 250;
				sprintf(msg, "当前楼层：%d", current_floor + 1);
				DrawText(hdc, msg, strlen(msg), &rt, DT_CENTER);

				EndPaint(hWnd, &ps);
			}
			break;
		case WM_KEYDOWN:
			if (GetAsyncKeyState(VK_F1)) {
				current_floor = 0;	InvalidateRect(ghWnd, NULL, TRUE);
			}
			if (GetAsyncKeyState(VK_F2)) {
				current_floor = 1;	InvalidateRect(ghWnd, NULL, TRUE);
			}
			if (GetAsyncKeyState(VK_F3)) {
				current_floor = 2;	InvalidateRect(ghWnd, NULL, TRUE);
			}
			if (GetAsyncKeyState(VK_F4)) {
				current_floor = 3;	InvalidateRect(ghWnd, NULL, TRUE);
			}
			if (GetAsyncKeyState(VK_F5)) {
				current_floor = 4;	InvalidateRect(ghWnd, NULL, TRUE);
			}
			break;
		case WM_LBUTTONDOWN:
			GetCursorPos(&cursorPos);
			GetWindowPos(hWnd, &wx, &wy);
			x = (int) cursorPos.x - wx;
			y = (int) cursorPos.y - wy;
			for (i = 0; i != 26; ++i) {
				if (x >= floors[current_floor][i].position.left && x <= floors[current_floor][i].position.right &&
					y >= floors[current_floor][i].position.top  && y <= floors[current_floor][i].position.bottom) {
					room_off = i;
					DialogBox(hInst, (LPCTSTR)IDD_ORDER, hWnd, (DLGPROC)Order);
				}
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

LRESULT CALLBACK Order(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	CHAR str[1024];
	switch (message)
	{
		case WM_INITDIALOG:
			sprintf(str, "%d", current_floor + 1);
			SetDlgItemText(hDlg, IDC_EDIT_FLOOR, str);

			sprintf(str, "%d", floors[current_floor][room_off].room_id);
			SetDlgItemText(hDlg, IDC_EDIT_ROOMID, str);

			switch (floors[current_floor][room_off].type) {
				case SINGLE: strcpy(str, "标间"); break;
				case DOUBLE: strcpy(str, "双人间"); break;
				case TRIBLE: strcpy(str, "三人间"); break;
				case LUXURY: strcpy(str, "豪华间"); break;
				case SUIT:   strcpy(str, "套间"); break;
			}
			SetDlgItemText(hDlg, IDC_EDIT_TYPE, str);
			return TRUE;
		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				if (LOWORD(wParam) == IDOK) {
					CHAR num_str[64];
					int time, price;
					WORD num_len = (WORD) SendDlgItemMessage(hDlg, IDC_EDIT_TIME, EM_LINELENGTH, (WPARAM) 0, (LPARAM) 0);
					SendDlgItemMessage(hDlg, IDC_EDIT_TIME, EM_GETLINE, (WPARAM) 0,(LPARAM) num_str);
					num_str[num_len] = '\0';
					sscanf(num_str, "%d", &time);
					switch (floors[current_floor][room_off].type) {
						case SINGLE: price = 150 * time; break;
						case DOUBLE: price = 150 * time; break;
						case TRIBLE: price = 200 * time; break;
						case LUXURY: price = 450 * time; break;
						case SUIT:   price = 270 * time; break;
					}
					sprintf(str, "您当前选择的房间需付款%d元，是否支付？", price);
					if (MessageBox(ghWnd, str, "支付确认", MB_YESNO) == IDYES) {
						floors[current_floor][room_off].status = UNAVAILABLE;
						MessageBox(ghWnd, "支付成功", "消息", MB_OK);
						EndDialog(hDlg, LOWORD(wParam));
						InvalidateRect(ghWnd, NULL, TRUE);
					}
					else {
						MessageBox(ghWnd, "支付终止", "消息", MB_OK);
					}
				}
				else {
					EndDialog(hDlg, LOWORD(wParam));
					InvalidateRect(ghWnd, NULL, TRUE);
				}
				return TRUE;
			}
			break;
	}
    return FALSE;
}
