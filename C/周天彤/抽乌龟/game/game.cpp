// game.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "stdio.h"
#include<ctime>
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
HWND hWnd;
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

typedef BOOL (WINAPI *pfcdtInit)(int *, int *);
typedef BOOL (WINAPI *pfcdtDraw)(HDC, int x, int y, int card, int type, DWORD color);
typedef BOOL (WINAPI *pfcdtDrawEx)(HDC, int x, int y, int dx, int dy, int card, int type, DWORD color);
typedef BOOL (WINAPI *pfcdtAnimate)(HDC hdc, int cardback, int x, int y, int frame);
typedef void (WINAPI *pfcdtTerm) (void);

void inicards();
int cardw,cardh;

pfcdtInit		cdtInit;
pfcdtDraw		cdtDraw;
pfcdtDrawEx		cdtDrawEx;
pfcdtAnimate	cdtAnimate;
pfcdtTerm		cdtTerm;

//================================================================
BOOL g_flag = false;
int state_flag = -1;
/* 
	0 :电脑自动
	1 :
	2 : 玩家选牌
	3 : 等待
*/
int cpt_sel;//电脑选中的牌
int plyer_sel;//玩家选中的牌

int g_js = 0;
int g_cpt_soc=0;
int g_player_soc=0;


int g_wg;
typedef struct cardInfo{

	int num;
	int x;
	int y;
	int type ;
	//struct cardInfo * next;
}Node;




Node cpt_cards[25],cpt_cards_box[25];
Node player_cards[25],player_cards_box[25];


//=================

void Updatew()
{
	RECT rt;
	GetClientRect(hWnd, &rt);
	InvalidateRect(hWnd,&rt,1); 

}
int Isin_rect(POINT tr)
{
	POINT point;
	GetCursorPos(&point);   // 获取鼠标指针位置（屏幕坐标）
	ScreenToClient(hWnd, &point); // 将鼠标指针位置转换为窗口坐标
	
	if (point.x >= tr.x && point.x <=  tr.x+cardw && point.y >= tr.y && point.y <= tr.y+cardh)
	{
		return 1;
	}

	return 0 ;
}
int getplayercardsnum()
{ 
	int s = 0;
	for (int i=0 ; i< 25 ; i++)
	{
		
		if (player_cards[i].num >= 0)
		{
			s++;
		}
	}
	return s;
}
int getcptcardsnum()
{ 
	int s = 0;
	for (int i=0 ; i< 25 ; i++)
	{
		
		if (cpt_cards[i].num >= 0)
		{
			s++;
		}
	}
	return s;
}
int checkgameover()
{
	if (!getcptcardsnum())
	{
		return 1;
	}
	if (!getplayercardsnum())
	{
		return 2;
	}
	//电脑胜利
	if (g_cpt_soc >= 100 || g_player_soc <= -100)
	{
		return 3;
	}
	//玩家胜利
	if ( g_player_soc>= 100 || g_cpt_soc <= -100)
	{
		return 4;
	}
	return 0;
}
void resetpos()
{
	int j1 = 0;
	int j2 = 0;
	int j3 = 0;
	int j4 = 0;
	for (int i=0 ; i< 25 ; i++)
	{
		
		if (cpt_cards[i].num >= 0)
		{
			cpt_cards[i].x = 10 + (cardw - 40)*j1;
			j1++;
		}
		

		
		if (cpt_cards_box[i].num >= 0)
		{
			cpt_cards_box[i].x = 10 + (cardw - 30)*j2;
			j2++;
		}
		

		
		if (player_cards[i].num >= 0)
		{
			player_cards[i].x = 10 + (cardw - 40)*j3;
			j3++;
		}
		

		
		if (player_cards_box[i].num >= 0)
		{
			player_cards_box[i].x = 10 + (cardw - 30)*j4;
			j4++;
		}
		

	}
}
//电脑抽取
void do_cpt_sele()
{
	int s = 0 ;//玩家 手中的牌数量
	int t[25];
	for (int i =0 ;i<25;i++)
	{
		if(player_cards[i].num >=0 )
		{
			t[s] = i;
			s++;
		}
	}
	//
	int s_i = rand()%s;//获取玩家手中一张牌
	cpt_sel = player_cards[t[s_i]].num;
	player_cards[t[s_i]].type = 2;

	Updatew();
	
	SetTimer(hWnd,0,2000,NULL);

}

void do_cpt_sele_2()
{
	//删除 玩家手中 被电脑选中的牌
	for (int i =0 ;i<25;i++)
	{
		if(player_cards[i].num >=0 )
		{
			if(player_cards[i].num/4 == cpt_sel/4 )
			{
				player_cards[i].num = -1;
				player_cards[i].type = 0;
				break;
			}
		}
	}
	



	//检查 电脑手中的牌
	bool flag = false;
	for (i =0 ;i<25;i++)
	{
		if(cpt_cards[i].num >=0 )
		{
			if(cpt_cards[i].num/4 == cpt_sel/4 )
			{
				cpt_cards[i].num = -1;
				flag = true;
				break;
			}
		}
	}

	if (flag)//手中有相同的牌
	{
		for (int i =0 ;i<25;i++)
			{
				if(cpt_cards_box[i].num >=0 )
				{
				
				}else //加入到收纳
				{
					cpt_cards_box[i].num = (cpt_sel/4)*4+1;
					break;

				}
			}

		for (  ;i<25;i++)
			{
				if(cpt_cards_box[i].num >=0 )
				{
				
				}else //加入到收纳
				{
					cpt_cards_box[i].num = (cpt_sel/4)*4+2;

					if(cpt_sel/4 == 0)  //A
					{
						g_cpt_soc += 15*2;
					}else if (cpt_sel/4 >0 && cpt_sel/4 <9)
					{
						g_cpt_soc += ((cpt_sel/4)+1)*2;
					}else
						g_cpt_soc += 10*2;
					
					
					break;

				}
			}
	
	
	}else{
		if ((cpt_sel/4) == (g_wg/4))//玩家抽到乌龟牌
		{
			g_cpt_soc -= 50;
		}


		for (int i =0 ;i<25;i++)
			{
				if(cpt_cards[i].num >=0 )
				{
				
				}else
				{
					cpt_cards[i].num = cpt_sel;
					break;

				}
			}
		
	}
	state_flag = 2;//玩家选
	Updatew();


		if(checkgameover())
		{
			char tt[100];
			if (checkgameover() == 1)
			{
				sprintf(tt,"游戏结束 电脑胜利_无牌");
			}else if (checkgameover() == 2)
			{
				sprintf(tt,"游戏结束 玩家胜利_无牌");
			}else if (checkgameover() == 3)
			{
				sprintf(tt,"游戏结束 电脑胜利");
			}else if (checkgameover() == 4)
			{
				sprintf(tt,"游戏结束 玩家胜利");
			}
			
			MessageBox(hWnd,tt,"",strlen(tt));

			inicards();	
		}

}
//玩家抽取
void do_player_sele()
{
	//删除 电脑手中 被玩家选中的牌
	for (int i =0 ;i<25;i++)
	{
		if(cpt_cards[i].num >=0 )
		{
			if(cpt_cards[i].num/4 == plyer_sel/4 )
			{
				cpt_cards[i].num = -1;
				cpt_cards[i].type = 1;
				break;
			}
		}
	}
	//检查 玩家手中的牌
	bool flag = false;
	for (i =0 ;i<25;i++)
	{
		if(player_cards[i].num >=0 )
		{
			if(player_cards[i].num/4 == plyer_sel/4 )
			{
				player_cards[i].num = -1;
				flag = true;
				break;
			}
		}
	}

	if (flag)//手中有相同的牌
	{
		for (int i =0 ;i<25;i++)
			{
				if(player_cards_box[i].num >=0 )
				{
				
				}else //加入到收纳
				{
					player_cards_box[i].num = (plyer_sel/4)*4+1;
					break;

				}
			}

		for ( i =0 ;i<25;i++)
			{
				if(player_cards_box[i].num >=0 )
				{
				
				}else //加入到收纳
				{
					player_cards_box[i].num = (plyer_sel/4)*4+2;

					if(plyer_sel/4 == 0)  //A
					{
						g_player_soc += 15*2;
					}else if (plyer_sel/4 >0 && plyer_sel/4 <9)
					{
						g_player_soc += ((plyer_sel/4)+1)*2;
					}else
						g_player_soc += 10*2;
					
					
					break;

				}
			}
	
	
	}else{
		if ((plyer_sel/4) == (g_wg/4))//玩家抽到乌龟牌
		{
			g_player_soc -= 50;
		}


		for (int i =0 ;i<25;i++)
			{
				if(player_cards[i].num >=0 )
				{
				
				}else
				{
					player_cards[i].num = plyer_sel;
					break;

				}
			}
		
	}

	Updatew();

		if(checkgameover())
		{
			char tt[100];
			if (checkgameover() == 1)
			{
				sprintf(tt,"游戏结束 电脑胜利_无牌");
			}else if (checkgameover() == 2)
			{
				sprintf(tt,"游戏结束 玩家胜利_无牌");
			}else if (checkgameover() == 3)
			{
				sprintf(tt,"游戏结束 电脑胜利");
			}else if (checkgameover() == 4)
			{
				sprintf(tt,"游戏结束 玩家胜利");
			}
			
			MessageBox(hWnd,tt,"",strlen(tt));

			inicards();	
		}
}
void dlsz_playercards()//打乱玩家手中的牌
{
	int t = 100;
	while(t--)
	{
		int i = rand()%25;
		int j= rand()%25;
		
		Node temp = player_cards [i];
		player_cards [i] = player_cards [j];
		player_cards [j] = temp;
	}

}
void dlsz_cptcards()//打乱玩家手中的牌
{
	int t = 100;
	while(t--)
	{
		int i = rand()%25;
		int j= rand()%25;
		
		Node temp = cpt_cards [i];
		cpt_cards [i] = cpt_cards [j];
		cpt_cards [j] = temp;
	}

}
void inicards()
{

	g_cpt_soc = 0;
	g_player_soc = 0;
	g_js ++;
	for (int i=0 ; i<25 ; i++)
	{
		cpt_cards[i].num = -1;
		cpt_cards_box[i].num = -1;
		player_cards[i].num = -1;
		player_cards_box[i].num = -1;
	}

	char dd[100];
	sprintf(dd,"%d",rand());
	
///	MessageBox(hWnd,dd,"",100);
	long start_player;
	srand((unsigned)time(&start_player)) ;
//	int start_player = srand((unsigned)time(NULL)) ()%3; //0 电脑 先开始
	start_player = start_player%2;
	for (i=0 ; i< 25 ; i++)
	{
		if (start_player == 1)
		{
			if (i<12)
			{
				cpt_cards[i].num = 1 + 4*i ;
				
			}
			
			if (i<13)
			{
				player_cards[i].num = 2 + 4*i ;
				
			}	
			state_flag = 0;
			
		}else
		{
			if (i<13)
			{
				cpt_cards[i].num = 1 + 4*i ;//i + 12*1 ;
			
			}
			
			if (i<12)
			{
				player_cards[i].num = 2 + 4*i ;
			
			}	

			state_flag = 2;
		}
		cpt_cards[i].y = 200;
		cpt_cards[i].type = 1;
		player_cards[i].y = 400;
		player_cards[i].type = 0;
		cpt_cards_box[i].y = 100;
		cpt_cards_box[i].type = 0;
		player_cards_box[i].y = 300;
		player_cards_box[i].type = 0;
	}
	dlsz_cptcards();
	dlsz_playercards();
	if(start_player == 1)
	{
		g_wg = 12*4;

		do_cpt_sele();
		
	}
	Updatew();
	

	
}
//===================
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	for (int i=0 ; i<25 ; i++)
	{
		cpt_cards[i].num = -1;
		cpt_cards_box[i].num = -1;
		player_cards[i].num = -1;
		player_cards_box[i].num = -1;
	}

	HMODULE hCardDll;
	hCardDll = LoadLibrary("cards.dll");
	if (hCardDll == 0)
	{
		return false;
	}

		//
	cdtInit    = (pfcdtInit)    GetProcAddress(hCardDll, "cdtInit");
	cdtDraw    = (pfcdtDraw)    GetProcAddress(hCardDll, "cdtDraw");
	cdtDrawEx  = (pfcdtDrawEx)  GetProcAddress(hCardDll, "cdtDrawExt");
	cdtAnimate = (pfcdtAnimate) GetProcAddress(hCardDll, "cdtAnimate");
	cdtTerm    = (pfcdtTerm)    GetProcAddress(hCardDll, "cdtTerm");

	cdtInit(&cardw,&cardh);
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;//LoadIcon(hInstance, (LPCTSTR)IDI_GAME);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//(LPCSTR)IDC_GAME;
	wcex.lpszClassName	= "game";//szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	RegisterClassEx(&wcex);

	

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow("game", "抓乌龟", WS_OVERLAPPEDWINDOW,
   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	//=============卡牌初始化=========
	inicards();

	g_flag = true;
	
	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
	}


	FreeLibrary(hCardDll);
	return 0;
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
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_KEYDOWN:
			if(state_flag == 4)
			{
				if (wParam == VK_SPACE)
				{
					state_flag = 0; // 电脑选择
					do_cpt_sele();
					dlsz_playercards();
					Updatew();
				}
			}
		break;
		case WM_TIMER:
			{
				if(wParam == 0)//选择 玩家手中牌等待显示2秒 后的操作
				{
					KillTimer(hWnd,0);
					do_cpt_sele_2();
				}
				if (wParam == 3)
				{

					KillTimer(hWnd,3);
					do_player_sele();
					//MessageBox(hWnd,"1","",1);
					state_flag = 4;//等待 玩家空格键;
				}

			}
		break;
		case WM_LBUTTONDOWN:
			{
				if(state_flag == 2)		//选牌状态
				{
					for(int i = 24 ; i >=0 ;i--)
					{
						
						if (cpt_cards[i].num >= 0 )
						{
							POINT tr;
							tr.x = cpt_cards[i].x;
							tr.y = cpt_cards[i].y;
							if (Isin_rect(tr))
							{
								plyer_sel = cpt_cards[i].num; //选中的num

								cpt_cards[i].type = 0;

								state_flag = 3; //显示等待
								SetTimer(hWnd,3,1000,NULL);
								Updatew();
								break; //
							}

							
						}

					}
				}
			}
		break;
		case WM_PAINT:
			{
				hdc = BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code here...

				resetpos();
				
				for(int i =0;i<25;i++)
				{
			
				
						if (cpt_cards_box[i].num != -1)
						{
						//	MessageBox(hWnd,"1","",1);
							if (cpt_cards_box[i].type == 1)
							{
								cdtDraw(hdc,cpt_cards_box[i].x,cpt_cards_box[i].y,56,cpt_cards_box[i].type,0);
							}else
							{
								cdtDraw(hdc,cpt_cards_box[i].x,cpt_cards_box[i].y,cpt_cards_box[i].num,cpt_cards_box[i].type,0);
							}
						}
					
						if (cpt_cards[i].num != -1)
						{
						//	MessageBox(hWnd,"1","",1);
							if (cpt_cards[i].type == 1)
							{
								cdtDraw(hdc,cpt_cards[i].x,cpt_cards[i].y,56,cpt_cards[i].type,0);
							}else
							{
								cdtDraw(hdc,cpt_cards[i].x,cpt_cards[i].y,cpt_cards[i].num,cpt_cards[i].type,0);
							}
						}
					


						if (player_cards[i].num != -1)
						{
						//	MessageBox(hWnd,"1","",1);
							if (player_cards[i].type == 1)
							{
								cdtDraw(hdc,player_cards[i].x,player_cards[i].y,56,player_cards[i].type,0);
							}else
							{
								cdtDraw(hdc,player_cards[i].x,player_cards[i].y,player_cards[i].num,player_cards[i].type,0);
							}
						}


						if (player_cards_box[i].num != -1)
						{
						//	MessageBox(hWnd,"1","",1);
							if (player_cards_box[i].type == 1)
							{
								cdtDraw(hdc,player_cards_box[i].x,player_cards_box[i].y,56,player_cards_box[i].type,0);
							}else
							{
								cdtDraw(hdc,player_cards_box[i].x,player_cards_box[i].y,player_cards_box[i].num,player_cards_box[i].type,0);
							}
						}

				}
						
					
						
			
				RECT rt;
				GetClientRect(hWnd, &rt);
				sprintf(szHello,"玩家得分:%d",g_player_soc);
				rt.left = rt.right -200;
				DrawText(hdc,szHello, strlen(szHello), &rt, DT_CENTER);
				
				rt.top = 40;
				sprintf(szHello,"电脑得分:%d",g_cpt_soc);
				DrawText(hdc,szHello, strlen(szHello), &rt, DT_CENTER);

				rt.top = 80;
				sprintf(szHello,"当前局数:%d",g_js);
				DrawText(hdc,szHello, strlen(szHello), &rt, DT_CENTER);
			
				

				EndPaint(hWnd, &ps);
				
				
				
				
			
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

