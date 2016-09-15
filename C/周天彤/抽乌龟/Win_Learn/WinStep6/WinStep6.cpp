// WinStep6.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
HWND hWnd;
LRESULT CALLBACK MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
typedef BOOL (WINAPI *pfcdtInit)(int *,int *);
typedef BOOL (WINAPI *pfcdtDraw)(HDC, int x, int y, int card , int type , DWORD color);
typedef BOOL (WINAPI *pfcdtDrawEx)(HDC, int x, int y, int dx, int dy ,int card, int type , DWORD color);
typedef BOOL (WINAPI *pfcdtAnimate)(HDC hdc,int cardback, int x, int y, int state);
typedef void (WINAPI *pfcdtTerm)(void);
pfcdtInit cdtInit;
pfcdtDraw cdtDraw;
pfcdtDrawEx cdtDrawEx;
pfcdtAnimate cdtAnimate;
pfcdtTerm cdtTerm;

int cardWidth,cardHeight;
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	HMODULE hCardDll;
	hCardDll=LoadLibrary("cards.dll");
	if(hCardDll==0)
		return FALSE;
    cdtInit  =(pfcdtInit)  GetProcAddress(hCardDll,"cdtInit");
	cdtDraw  =(pfcdtDraw)  GetProcAddress(hCardDll,"cdtDraw");
	cdtDrawEx  =(pfcdtDrawEx)  GetProcAddress(hCardDll,"cdtDrawEx");
	cdtAnimate  =(pfcdtAnimate)  GetProcAddress(hCardDll,"cdtAnimate");
	cdtTerm  =(pfcdtTerm)  GetProcAddress(hCardDll,"cdtTerm");

	cdtInit(&cardWidth,&cardHeight);

 	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)MainProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "WIN_step6";
	wcex.hIconSm		= NULL;

	RegisterClassEx(&wcex);
    hWnd = CreateWindow("WIN_step6", "Ö½ÅÆÑÝÊ¾", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    MSG msg;
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	FreeLibrary(hCardDll);
	 return 0;
}
LRESULT CALLBACK MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rcClient;
	switch (message) 
	{
	case WM_PAINT:
			hdc=BeginPaint(hWnd,&ps);
			GetClientRect(hWnd,&rcClient);
			cdtDraw(hdc,20,20,50,0,0);
			EndPaint(hWnd,&ps);
			break;
	case WM_DESTROY:
			PostQuitMessage(0);
			break;
	default:
			return DefWindowProc(hWnd,message,wParam,lParam);
	}
	return 0;
}


