#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// 全局变量
HINSTANCE hInst;	// 当前实例
HWND      ghWnd;	// 主窗体句柄
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];

static float required_price = 0.f;			// 应收款
static float paid_price = 0.f;				// 实际付款
static float totalsum = 0.f;				// 今日总计
enum {req, cash, cancle, sum} displaying;	// 当前 POS 屏幕显示的内容

// 找零，直接用宏不用变量了
#define cash_price (paid_price - required_price)

// 一些声明
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	InputPrice(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	InputPaid(HWND, UINT, WPARAM, LPARAM);
void Draw_Mini_Char(HDC hdc, int x, int y, int base, char ch, COLORREF textColor, COLORREF bgColor);
void Draw_Mini_String(HDC hdc, int x, int y, int base, CHAR *s, COLORREF textColor, COLORREF bgColor);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_POSEMULATOR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化，显示窗体
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	// 加载快捷键相关
	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_POSEMULATOR);

	// 主消息循环
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
//  函数: MyRegisterClass()
//
//  用途: 注册窗体类.
//
//  注释:
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
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_POSEMULATOR);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_POSEMULATOR;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HANDLE, int)
//
//   用途: Saves instance handle and creates main window
//
//   注释:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   hInst = hInstance; // 将实例句柄保存到全局变量

   // 创建窗体并保存到全局变量
   ghWnd = hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 450, 390, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, unsigned, WORD, LONG)
//
//  用途:  处理主窗体上的消息事件.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	CHAR MSG[64];

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// 分析菜单选择项:
			switch (wmId)
			{
				case IDM_ABOUT:				// 关于
				    DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				    break;
				case IDM_EXIT:				// 退出
				    DestroyWindow(hWnd);
				    break;
				case IDM_INPUT_PRICE:		// 输入应付款
					DialogBox(hInst, (LPCTSTR)IDD_DIALOG_INPUT_PRICE, hWnd, (DLGPROC)InputPrice);
					break;
				case IDM_INPUT_PAID:		// 输入实付款
					DialogBox(hInst, (LPCTSTR)IDD_DIALOG_INPUT_PAID, hWnd, (DLGPROC)InputPaid);
					break;
				case IDM_CANCLE:			// 取消当前交易
					displaying = cancle;
					required_price = 0.f;
					paid_price = 0.f;
					InvalidateRect(ghWnd, NULL, TRUE);
					break;
				case IDM_COMMIT:			// 确认当前交易
					totalsum += cash_price;
					required_price = 0.f;
					paid_price = 0.f;
					displaying = req;
					InvalidateRect(ghWnd, NULL, TRUE);
					break;
				case IDM_SHOWSUM:			// 显示日交易额
					displaying = sum;
					InvalidateRect(ghWnd, NULL, TRUE);
					break;
				case IDM_REBOOT:			// 重置 POS
					required_price = 0.f;
					paid_price = 0.f;
					totalsum = 0.f;
					displaying = req;
					InvalidateRect(ghWnd, NULL, TRUE);
					break;
				default:					// 其他无关事件
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// 根据要显示的内容，输出相应数值和POS图形
			switch (displaying) {
			case req:	
				sprintf(MSG, "$%06.2f", required_price);
				Draw_Mini_String(hdc, 10, 10, 10, MSG, PALETTERGB(0, 0, 0), PALETTERGB(255, 255, 255));
				sprintf(MSG, "当前显示：应付款");
				break;
			case cash:
				sprintf(MSG, "$%06.2f", paid_price);
				Draw_Mini_String(hdc, 10, 10, 10, MSG, PALETTERGB(0, 0, 0), PALETTERGB(255, 255, 255));
				sprintf(MSG, "$%06.2f", cash_price);
				Draw_Mini_String(hdc, 10, 150, 10, MSG, PALETTERGB(0, 0, 0), PALETTERGB(255, 255, 255));
				sprintf(MSG, "当前显示：实付款");
				break;
			case cancle:
				sprintf(MSG, "CANCLE");
				Draw_Mini_String(hdc, 10, 10, 10, MSG, PALETTERGB(0, 0, 0), PALETTERGB(255, 255, 255));
				sprintf(MSG, "当前显示：取消交易");
				break;
			case sum:
				sprintf(MSG, "$%06.2f", totalsum);
				Draw_Mini_String(hdc, 10, 10, 10, MSG, PALETTERGB(0, 0, 0), PALETTERGB(255, 255, 255));
				sprintf(MSG, "当前显示：今日总计");
				break;
			}
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, MSG, strlen(MSG), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if (GetAsyncKeyState(VK_F12)) {
				CHAR message[1024];
				sprintf(message, "Value in ram:\n\trequited_price: %.3f.\n\tpaid_price: %.3f.\n\ttotal_sum: %.3f", required_price, paid_price, totalsum);
				MessageBox(hWnd, message, "info", MB_OK);
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

// From https://github.com/ntzyz/nNovel-Plus/blob/master/graph.c, modified
// 在 hdc 的 [x, y] 位置显示一个 6 * 8 的字符，放大倍数是 base 倍，字符是 ch，textColor 和 bgColor 是字的颜色
void Draw_Mini_Char(HDC hdc, int x, int y, int base, char ch, COLORREF textColor, COLORREF bgColor) {
	int i, j, pixelOn, m, n;
	for(i = 0; i < 6; i++) {
		for(j = 8; j > 0; j--) {
			pixelOn = charMap_ascii_mini[(unsigned char)ch][i] << j ;
			pixelOn = pixelOn & 0x80 ;
			for (m = 0; m != base; ++m)
				for (n = 0; n != base; ++n) {
					SetPixel(hdc, x + i * base + m, y + (8 - j) * base + n, pixelOn ? textColor : bgColor);
				}
		}
	}
}

// From https://github.com/ntzyz/nNovel-Plus/blob/master/graph.c, modified
// 在 hdc 的 [x, y] 位置显示一组 6 * 8 的字符，放大倍数是 base 倍，字符串是 s，textColor 和 bgColor 是字的颜色
void Draw_Mini_String(HDC hdc, int x, int y, int base, CHAR *s, COLORREF textColor, COLORREF bgColor) {
	for (;*s; ++s) {
		Draw_Mini_Char(hdc, x, y, base, *s, textColor, bgColor);
		x += base * 6;
	}
}

// 关于对话框的消息处理函数
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

// 输入付款对话框的消息处理函数
LRESULT CALLBACK InputPrice(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK) {	// 按下确认键
				CHAR num_str[64];
				WORD num_len = (WORD) SendDlgItemMessage(hDlg, 
                                                         IDC_EDIT1, 
                                                         EM_LINELENGTH, 
                                                         (WPARAM) 0, 
                                                         (LPARAM) 0); 
				if (num_len == 0) {
					// 长度为零，即没有输入字符时，弹出相应提示
					MessageBox (hDlg, "请输入正确的金额", "ERROR", MB_OK);
					return TRUE;
				}
				else {
					// 输入了数字，计算输入值并保存到全局变量

					// 拉取输入的字符
					SendDlgItemMessage(hDlg, 
                                       IDC_EDIT1, 
                                       EM_GETLINE, 
                                       (WPARAM) 0,
                                       (LPARAM) num_str);
					// 补全最后的 '\0'
					num_str[num_len] = '\0';
					// 用 sscanf 获得浮点数值
					sscanf(num_str, "%f", &required_price);
					// 修改显示的内容为应付款
					displaying = req;
					// 结束当前对话框
					EndDialog(hDlg, LOWORD(wParam));
					// 触发 WM_PAINT 事件
					InvalidateRect(ghWnd, NULL, TRUE);
					return FALSE;
				}
			}
			if (LOWORD(wParam) == IDCANCEL) {
				// 取消键，不做任何处理直接返回即可
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

// 输入实付款对话框的消息处理函数，和上一个比较类似
LRESULT CALLBACK InputPaid(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK) {
				CHAR num_str[64];
				WORD num_len = (WORD) SendDlgItemMessage(hDlg, 
                                                         IDC_EDIT1, 
                                                         EM_LINELENGTH, 
                                                         (WPARAM) 0, 
                                                         (LPARAM) 0); 
				if (num_len == 0) {
					MessageBox (hDlg, "请输入正确的金额", "ERROR", MB_OK);
					return TRUE;
				}
				else {
					SendDlgItemMessage(hDlg, 
                                       IDC_EDIT1, 
                                       EM_GETLINE, 
                                       (WPARAM) 0,
                                       (LPARAM) num_str);
					num_str[num_len] = '\0';
					sscanf(num_str, "%f", &paid_price);
					displaying = cash;
					EndDialog(hDlg, LOWORD(wParam));

					InvalidateRect(ghWnd, NULL, TRUE);
					return FALSE;
				}
			}
			if (LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}