// DS1041hw2_10327146_10327108.cpp : 定义应用程式的进入点。
//

#include "stdafx.h"
#include "SudokuGame.h"

#define MAX_LOADSTRING 100
#define WINDOWX 800
#define WINDOWY 600
#define MYTIMER 0
#define COUNTTIME_MIN 3 // 3分钟
#define COUNTTIME_SEC 59 // 1 min -> 60sec
#define NUMOFTYPE 4

// 全域变数: 
HINSTANCE hInst;								// 目前执行个体
TCHAR szTitle[MAX_LOADSTRING] = L"Sudoku";					// 标题列文字
TCHAR szWindowClass[MAX_LOADSTRING] = L"Sudoku";			// 主视窗类别名称
HWND hwnd, hAbout;
WPARAM wPARAM;
POINT ScreenSize;
int currentQuestion;
dataSudoku sudoku;
DataList sudokuList;
orderSudoku sudokuOrder;
PlayerRank thePlayer;
RankList rankList;

int selectedBlockIndex = -1;

void checkError(HWND hWnd);
TCHAR* convertIntToL(int num);

void DrawSudoku(PAINTSTRUCT ps, HDC hdc);
void DrawTime(PAINTSTRUCT ps, HDC hdc);
void DrawRank(PAINTSTRUCT ps, HDC hdc);
void DrawTutorial(PAINTSTRUCT ps, HDC hdc);
HFONT MyCreateFont(int nHeight, DWORD dwCharSet, LPCTSTR);

int xyToIndex(POINT po);

void  goBackTo(char*);

void setShowSudoku(char*);
void slove1All();
void slove1Add();
void slove1Show();
void slove1Delete();

void slove2All();
void slove2Play();
void slove2Practice();
void slove2Rank();
bool slove2Send();

UINT MyTimer;
void endTimer();
int startTime_Min = 0, startTime_Sec = 0;
bool inSlove1 = false, inSlove1Show = false, inSlove1Delete = false, showSudoku = false;
bool inSlove2 = false, startPlay = false, needTimer = false, showRank = false;

INT_PTR CALLBACK sudokuMaker(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK sudokuManeger(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK sudokuPlay(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK sudokuRank(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK sudokuAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int maxNumToAdd[10] = { 100, 50, 25, 15, 10, 7, 5, 2, 1, 1 };
/****************************** class ImgButton ************************************/
class ImgButton {

public:
	int width;
	int height;
	POINT po; // position
	char order[100];
	void theOrder();
	HDC hdc;
	ImgButton(int width, int height, POINT po, char*);
	ImgButton();
	void loadImage(HDC, int);
	void excuteOrder();
	bool allowExcute;
};

ImgButton::ImgButton() { allowExcute = false; } //
ImgButton::ImgButton(int Width, int Height, POINT Po, char* Order) {

	width = Width;
	height = Height;
	po = Po;
	strcpy_s(order, sizeof(order), Order);
	allowExcute = false;
} //

void ImgButton::loadImage(HDC hhdc, int rc) {
	hdc = CreateCompatibleDC(hhdc);
	HBITMAP hBmp = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(rc), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);//LoadBitmap( hInst, lp );LR_DEFAULTCOLOR  LR_LOADFROMFILE
	//checkError(NULL);
	SelectObject(hdc, hBmp);
	DeleteObject(hBmp);
} // 

void ImgButton::excuteOrder() {

	if (strcmp(order, "goBack") == 0) {
		if ((inSlove1 || inSlove2) && !showSudoku && !showRank) goBackTo("sloveAll"); // 
		else if (inSlove1 && showSudoku)
			goBackTo("slove1"); // 
		else if (inSlove2 && (startPlay || showRank))
			goBackTo("slove2"); // 
		else MessageBox(NULL, L"other order", L"Error", MB_OK);
	} // if
	/****************************** Slove1 ************************************/
	else if (strcmp(order, "slove1") == 0 && !inSlove1) {
		inSlove1 = true;
		slove1All();
	} // if
	else if (strcmp(order, "slove1Add") == 0 && inSlove1) {
		slove1Add();
	} // else if
	else if (strcmp(order, "slove1Show") == 0 && inSlove1) {
		slove1Show();
	} // else if
	else if (strcmp(order, "slove1Delete") == 0 && inSlove1) {
		slove1Delete();
	} // else if
	/****************************** Slove2 ************************************/
	else if (strcmp(order, "slove2") == 0 && !inSlove2) {
		inSlove2 = true;
		slove2All();
	} // if
	else if (strcmp(order, "slove2Play") == 0 && inSlove2) {
		slove2Play();
	} // if
	else if (strcmp(order, "slove2Practice") == 0 && inSlove2) {
		slove2Practice();
	} // if
	else if (strcmp(order, "slove2Rank") == 0 && inSlove2) {
		slove2Rank();
	} // if
	else if (strcmp(order, "slove2Send") == 0 && inSlove2) {
		slove2Send();
	} // if
}

/****************************** End class ImgButton ************************************/
void beenClick(LPARAM, ImgButton**, int);


// 这个程式码模组中所包含之函式的向前宣告: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO:  在此置入程式码。
	MSG msg;

	// 初始化全域字串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程式初始设定: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}


	// 主讯息回圈: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	return (int)msg.wParam;
}



//
//  函式:  MyRegisterClass()
//
//  用途:  注册视窗类别。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SUDOKUGAME));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx(&wcex);
}

//
//   函式:  InitInstance(HINSTANCE, int)
//
//   用途:  储存执行个体控制代码并且建立主视窗
//
//   注解: 
//
//        在这个函式中，我们会将执行个体控制代码储存在全域变数中，
//        并且建立和显示主程式视窗。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 将执行个体控制代码储存在全域变数中

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION,
		CW_USEDEFAULT, 0, WINDOWX, WINDOWY, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	return TRUE;
}


void checkError(HWND hWnd) {

	static LPTSTR Error = 0;
	static DWORD err;
	err = GetLastError();

	// Translate ErrorCode to String.

	if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		err,
		0,
		(LPTSTR)& Error,
		0,
		NULL) == 0)
	{
		// Failed in translating.
	}

	// Display message.
	MessageBox(hWnd, Error, _T("GetCurrentDirectory Error"), MB_OK | MB_ICONWARNING);
}

//
//  函式:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  用途:     处理主视窗的讯息。
//
//  WM_COMMAND	- 处理应用程式功能表
//  WM_PAINT	- 绘制主视窗
//  WM_DESTROY	- 显示结束讯息然后返回
//
//

ImgButton background(8, 6, { 0, 0 }, ""), reBackground(8, 6, { 0, 0 }, "");
ImgButton title(700, 138, { 50, 100 }, "");

ImgButton* goBack = new ImgButton(100, 80, { 740 - 60, 480 }, "goBack");

ImgButton* slove1 = new ImgButton(305, 100, { 240, 420 }, "slove1"), * slove1_Add = new ImgButton(300, 100, { 240, 150 }, "slove1Add"),
* slove1_Show = new ImgButton(300, 100, { 240, 250 }, "slove1Show"), * slove1_Delete = new ImgButton(300, 100, { 240, 350 }, "slove1Delete");
HDC numbersHDC[10];

ImgButton* slove2 = new ImgButton(305, 100, { 240, 320 }, "slove2"), * slove2_Practice = new ImgButton(300, 89, { 240, 250 }, "slove2Practice"),
* slove2_Play = new ImgButton(300, 100, { 240, 150 }, "slove2Play"), * slove2_Rank = new ImgButton(300, 100, { 240, 350 }, "slove2Rank"),
* slove2_Send = new ImgButton(200, 160, { 570, 100 }, "slove2Send");
void showAbout();
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	static HDC hdc, mhdc = NULL;
	static HWND hDlg;
	static ImgButton* buttonList[10];
	static int numList = 0;
	static HPEN hPen, hOldPen;
	static HBRUSH hBrush, hOldBrush;
	static HBITMAP hBmp;
	static Huffman_Order huffmanOrder;
	static int tempIndex;
	switch (message)
	{
		/*
		case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 剖析功能表选取项目:
		switch (wmId)
		{
		case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
		case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
		default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
		*/
	case WM_CREATE:
		showAbout();
		huffmanOrder.order = "decompress";
		huffmanOrder.srcFileName = "dataSudoku.data";
		if (!huffmanCompression(huffmanOrder)) {
			DestroyWindow(hAbout);
			MessageBox(hwnd, L"尚未建立数据或纪录档遗失", L"!!!!!", MB_OK);
		} // if
		huffmanOrder.srcFileName = "sudokuRank.data";
		if (!huffmanCompression(huffmanOrder)) {
			DestroyWindow(hAbout);
			MessageBox(hwnd, L"尚未建立排名或排名纪录遗失", L"!!!!!", MB_OK);
		} // if
		else DestroyWindow(hAbout);

		hwnd = hWnd;
		wPARAM = wParam;
		hdc = GetDC(hWnd);
		mhdc = CreateCompatibleDC(hdc);
		/***************************************/

		background.loadImage(hdc, IMG_BACKGROUND);
		reBackground.loadImage(hdc, IMG_BACKGROUND);
		title.loadImage(hdc, IMG_title);

		goBack->loadImage(hdc, IMG_Back);
		buttonList[numList++] = goBack;

		/////////////////slove1

		slove1->allowExcute = true;
		slove1->loadImage(hdc, IMG_slove1);
		buttonList[numList++] = slove1;
		slove1_Add->loadImage(hdc, IMG_slove1Add);
		buttonList[numList++] = slove1_Add;
		slove1_Show->loadImage(hdc, IMG_slove1Show);
		buttonList[numList++] = slove1_Show;
		slove1_Delete->loadImage(hdc, IMG_slove1Delete);
		buttonList[numList++] = slove1_Delete;
		slove1_Add->allowExcute = false;
		slove1_Show->allowExcute = false;
		slove1_Delete->allowExcute = false;
		/////////////////slove2
		slove2->allowExcute = true;
		slove2->loadImage(hdc, IMG_slove2);
		buttonList[numList++] = slove2;
		slove2_Practice->loadImage(hdc, IMG_slove2Practice);
		buttonList[numList++] = slove2_Practice;
		slove2_Play->loadImage(hdc, IMG_slove2Play);
		buttonList[numList++] = slove2_Play;
		slove2_Rank->loadImage(hdc, IMG_slove2Rank);
		buttonList[numList++] = slove2_Rank;
		slove2_Send->loadImage(hdc, IMG_slove2Send);
		buttonList[numList++] = slove2_Send;
		slove2_Practice->allowExcute = false;
		slove2_Play->allowExcute = false;
		slove2_Rank->allowExcute = false;
		slove2_Send->allowExcute = false;
		///////////////////////////

		ReleaseDC(hWnd, hdc);
		//SetWindowPos(hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);

		ScreenSize.x = 800;
		ScreenSize.y = 600;
		break;
	case WM_ERASEBKGND:
		break;

	case WM_PAINT:

		/*if (ScreenSize.x != GetSystemMetrics(SM_CXSCREEN) || ScreenSize.y !=  GetSystemMetrics(SM_CYSCREEN)){
			ScreenSize = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
			for (int i = 0; i < numList; i++) {
				buttonList[i]->po.x *= (ScreenSize.x / 800);
				buttonList[i]->po.y *= (ScreenSize.y / 600);
				buttonList[i]->width *= (ScreenSize.x / 800);
				buttonList[i]->height *= (ScreenSize.y / 600);
			} // for
		} // if*/

		hdc = BeginPaint(hWnd, &ps);
		SetBkColor(ps.hdc, RGB(200, 191, 231));

		BitBlt(reBackground.hdc, 0, 0, 800, 600, background.hdc, 0, 0, SRCCOPY);
		//StretchBlt(reBackground.hdc, background.po.x, background.po.y, 800, 600, background.hdc, 0, 0, background.width, background.height, SRCCOPY);//

		if (inSlove1 || inSlove2)
			BitBlt(reBackground.hdc, goBack->po.x, goBack->po.y, goBack->width, goBack->height, goBack->hdc, 0, 0, SRCCOPY);
		if (startPlay)
			BitBlt(reBackground.hdc, slove2_Send->po.x, slove2_Send->po.y, slove2_Send->width, slove2_Send->height, slove2_Send->hdc, 0, 0, SRCCOPY);
		if (inSlove2 && showRank)
			DrawRank(ps, reBackground.hdc);
		if (showSudoku || startPlay) {
			DrawSudoku(ps, reBackground.hdc);
			if (startPlay) DrawTutorial(ps, reBackground.hdc);
			if (needTimer) 	DrawTime(ps, reBackground.hdc);
		} // if
		else if (!inSlove1 && !inSlove2 && !showSudoku) {
			BitBlt(reBackground.hdc, title.po.x, title.po.y, title.width, title.height, title.hdc, 0, 0, SRCCOPY);
			BitBlt(reBackground.hdc, slove1->po.x, slove1->po.y, slove1->width, slove1->height, slove1->hdc, 0, 0, SRCCOPY);
			BitBlt(reBackground.hdc, slove2->po.x, slove2->po.y, slove2->width, slove2->height, slove2->hdc, 0, 0, SRCCOPY);
		} // else if
		else if (inSlove1 && !showSudoku) {
			BitBlt(reBackground.hdc, slove1_Add->po.x, slove1_Add->po.y, slove1_Add->width, slove1_Add->height, slove1_Add->hdc, 0, 0, SRCCOPY);
			BitBlt(reBackground.hdc, slove1_Show->po.x, slove1_Show->po.y, slove1_Show->width, slove1_Show->height, slove1_Show->hdc, 0, 0, SRCCOPY);
			BitBlt(reBackground.hdc, slove1_Delete->po.x, slove1_Delete->po.y, slove1_Delete->width, slove1_Delete->height, slove1_Delete->hdc, 0, 0, SRCCOPY);
		} // if
		else if (inSlove2 && !showRank) {
			BitBlt(reBackground.hdc, slove2_Practice->po.x, slove2_Practice->po.y, slove2_Practice->width, slove2_Practice->height, slove2_Practice->hdc, 0, 0, SRCCOPY);
			BitBlt(reBackground.hdc, slove2_Play->po.x, slove2_Play->po.y, slove2_Play->width, slove2_Play->height, slove2_Play->hdc, 0, 0, SRCCOPY);
			BitBlt(reBackground.hdc, slove2_Rank->po.x, slove2_Rank->po.y, slove2_Rank->width, slove2_Rank->height, slove2_Rank->hdc, 0, 0, SRCCOPY);
		}

		BitBlt(ps.hdc, 0, 0, ScreenSize.x, ScreenSize.y, reBackground.hdc, 0, 0, SRCCOPY);
		//StretchBlt(ps.hdc, reBackground.po.x, reBackground.po.y, ScreenSize.x, ScreenSize.y, reBackground.hdc, 0, 0, reBackground.width, reBackground.height, SRCCOPY);//BitBlt(ps.hdc, 0, 0, 800, 600, reBackground.hdc, 0, 0, SRCCOPY);
		//StretchBlt(ps.hdc, reBackground.po.x, reBackground.po.y, 800, 600, reBackground.hdc, 0, 0, reBackground.width, reBackground.height, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_MOUSEMOVE:
		if (GetCursor() != LoadCursor(NULL, IDC_HAND)) {
			for (int i = 0; i < numList; i++) {

				if (GET_X_LPARAM(lParam) > buttonList[i]->po.x && GET_X_LPARAM(lParam) < buttonList[i]->po.x + buttonList[i]->width &&
					GET_Y_LPARAM(lParam) > buttonList[i]->po.y && GET_Y_LPARAM(lParam) < buttonList[i]->po.y + buttonList[i]->height) {
					if (buttonList[i]->allowExcute) {
						if ((buttonList[i] == slove2_Rank && showRank) || (buttonList[i] == slove2_Send && !startPlay)) break;
						else {
							SetCursor(LoadCursor(NULL, IDC_HAND));
							break;

						}
					} // if
				} // if
			}
		} // if
		break;
	case WM_LBUTTONDOWN:
		beenClick(lParam, buttonList, numList);
		break;
	case WM_KEYDOWN:
		if (startPlay) {
			tempIndex = selectedBlockIndex;
			if (wParam == VK_LEFT && 0 < selectedBlockIndex)
				while (selectedBlockIndex >= 0 && !sudoku.Sudoku()[--selectedBlockIndex].hide);
			else if (wParam == VK_RIGHT && selectedBlockIndex < sudoku.Size() * sudoku.Size() - 1)
				while (selectedBlockIndex < sudoku.Size() * sudoku.Size() - 1 && !sudoku.Sudoku()[++selectedBlockIndex].hide);
			else if (wParam == VK_UP) {
				if (0 <= (selectedBlockIndex - sudoku.Size())) {
					selectedBlockIndex -= sudoku.Size();
					while (selectedBlockIndex >= sudoku.Size() && !sudoku.Sudoku()[selectedBlockIndex].hide) selectedBlockIndex -= sudoku.Size();
				}
			}
			else if (wParam == VK_DOWN) {
				if ((selectedBlockIndex + sudoku.Size()) < sudoku.Size() * sudoku.Size()) {
					selectedBlockIndex += sudoku.Size();
					while (selectedBlockIndex < sudoku.Size() * sudoku.Size() - sudoku.Size() && !sudoku.Sudoku()[selectedBlockIndex].hide) selectedBlockIndex += sudoku.Size();
				} // if
			}
			if (!sudoku.Sudoku()[selectedBlockIndex].hide || (sudoku.Size() * sudoku.Size() - 1 < selectedBlockIndex && selectedBlockIndex < 0)) selectedBlockIndex = tempIndex;
			InvalidateRect(hwnd, NULL, FALSE);
		} // if
		break;
	case WM_CHAR:
		if (startPlay && sudoku.Sudoku()[selectedBlockIndex].hide) {

			if (48 <= wParam && wParam <= 57) {// 0~9

				if ((int)(wParam - 48) > sudoku.Size()) break;

				if (sudoku.Sudoku()[selectedBlockIndex].num * 10 + (int)(wParam - 48) <= sudoku.Size())
					sudoku.Sudoku()[selectedBlockIndex].num = sudoku.Sudoku()[selectedBlockIndex].num * 10 + wParam - 48;
				else sudoku.Sudoku()[selectedBlockIndex].num = wParam - 48;

				InvalidateRect(hwnd, NULL, TRUE);
			} // if
			else if (wParam == VK_RETURN)
				slove2Send();
		}
		break;
	case WM_DESTROY:
		huffmanOrder.order = "compress"; huffmanOrder.srcFileName = "sudokuRank.data";  //排名压缩出问题
		huffmanCompression(huffmanOrder);
		huffmanOrder.srcFileName = "dataSudoku.data";
		huffmanCompression(huffmanOrder);
		for (int i = 0; i < numList; i++)
			delete buttonList[i];
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


void beenClick(LPARAM lp, ImgButton** list, int num) {
	POINT po;
	//GetCursorPos(&po);
	po.x = GET_X_LPARAM(lp), po.y = GET_Y_LPARAM(lp);

	/*
	int index1 = po.x, yy = po.y;
	std::string test1 = std::to_string(index1);
	std::string test2 = std::to_string(yy);
	MessageBoxA(NULL, test1.c_str(), test2.c_str(), MB_OK);
	*/
	if (startPlay) {

		selectedBlockIndex = xyToIndex(po);
		InvalidateRect(hwnd, NULL, FALSE);

		//TCHAR buffer[256];
		//wsprintf(buffer, TEXT("%d"), selectedBlockIndex);
		//MessageBox(NULL, buffer, L"", MB_OK);
	} // if

	for (int i = 0; i < num; i++) {
		if (po.x > list[i]->po.x && po.x < list[i]->po.x + list[i]->width && po.y > list[i]->po.y && po.y < list[i]->po.y + list[i]->height) {
			if (list[i]->allowExcute) {
				list[i]->excuteOrder();
				i = num;
			} // if
		}
	} // for


}

int xyToIndex(POINT po) {

	int offset = (WINDOWY - 40) / sudoku.Size();
	int row = -1, column = -1;

	for (int i = 0; i < sudoku.Size(); i++) {

		if (i * offset <= po.x && po.x <= (i + 1) * offset) column = i;
		if (i * offset <= po.y && po.y <= (i + 1) * offset) row = i;
	} // for


	if (column < 0 || row < 0) return -99;
	return row * sudoku.Size() + column;
}

POINT indexToXY(int i) {
	int dis = (WINDOWY - 40) / sudoku.Size();
	int column = i / sudoku.Size(), row = i % sudoku.Size();
	//row = i;
	//if ((i + 1) % (sudoku.Size()) == 0 && i != 0) column++;
	return{ row * dis, column * dis };
}

/*********************************** Draw **************************************************/

void DrawSudoku(PAINTSTRUCT ps, HDC hdc) {
	/************************** 格子 *******************************/
	int colorChange = 0;
	HPEN hPen, hBlock, hOldPen;
	HBRUSH hBrush[4], hOldBrush;
	POINT src, des, offset;
	int sqrtSize = (int)sqrt(sudoku.Size());
	offset.x = offset.y = (int)((WINDOWY - 40) / sudoku.Size()); // can use ceil
	src.x = 0, src.y = 0;
	des.x = des.y = (int)((WINDOWY - 40) / sudoku.Size());
	/************************** 数字 *******************************/
	COLORREF colorOLD = SetTextColor(hdc, RGB(0, 0, 0));
	int oldBackgroundMode = 0;
	POINT po;
	RECT rc;
	int value = 0;
	TCHAR theNumber[256];

	int dis = (int)((WINDOWY - 40) / sudoku.Size());
	//SetWindowPos(hwnd, HWND_TOP, 0, 0, 800, dis * (sudoku.Size()), SWP_NOMOVE);
	HFONT hFont = MyCreateFont(dis / 2, CHINESEBIG5_CHARSET, TEXT("标楷体")), hOldFont;
	hOldFont = (HFONT)SelectObject(hdc, hFont);
	/************************** Set Colors *******************************/

	hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	hBlock = CreatePen(PS_SOLID, 4, RGB(108, 51, 101));

	hOldPen = (HPEN)SelectObject(hdc, hPen);
	hBrush[0] = CreateSolidBrush(RGB(209, 209, 209));
	hBrush[1] = CreateSolidBrush(RGB(209, 209, 209));
	hBrush[2] = CreateSolidBrush(RGB(129, 65, 65)); // 选择时的格子
	hBrush[3] = CreateSolidBrush(RGB(250, 200, 150)); // 可选择的格子
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush[0]);

	/************************** 画数独 ******************************* (129, 65, 65) */
	oldBackgroundMode = SetBkMode(hdc, TRANSPARENT);
	int index = 0;

	//TCHAR buffer[256];
	//wsprintf(buffer, TEXT("%d"), selectedBlockIndex);
	//MessageBox(NULL, buffer, L"", MB_OK);
	for (int i = 0; i < sudoku.Size(); i++) {
		for (int j = 0; j < sudoku.Size(); j++) {

			index = i * sudoku.Size() + j;
			SelectObject(hdc, hPen);
			if (j % 2 == colorChange)
				SelectObject(hdc, hBrush[0]);
			else
				SelectObject(hdc, hBrush[1]);
			if (sudoku.Sudoku()[index].hide)
				SelectObject(hdc, hBrush[3]);
			if (startPlay && selectedBlockIndex >= 0 && index == selectedBlockIndex && (sudoku.Sudoku()[index].hide)) {
				SelectObject(hdc, hBrush[2]);

				//MessageBox(hwnd, L"", L"", MB_OK);
			} // if

			Rectangle(hdc, src.x, src.y, src.x + offset.x, src.y + offset.y);
			/**************** Draw Block ***************/
			if (j % sqrtSize == 0) {
				SelectObject(hdc, hBlock);
				MoveToEx(hdc, src.x, 0, NULL);
				LineTo(hdc, src.x, des.y * sudoku.Size() - 1);
			}
			if (i % sqrtSize == 0) {
				SelectObject(hdc, hBlock);
				MoveToEx(hdc, 0, src.y, NULL);
				LineTo(hdc, des.x * sudoku.Size() - 2, src.y);
			}

			if (!sudoku.Sudoku()[index].hide || (sudoku.Sudoku()[index].hide && sudoku.Sudoku()[index].num > 0)) {
				po = indexToXY(index);
				value = sudoku.Sudoku()[index].value();
				wsprintf(theNumber, TEXT("%d"), value);
				rc = { po.x, po.y + dis / 4, po.x + dis, po.y + dis };
				DrawText(hdc, theNumber, -1, &rc, DT_CENTER);
			} // if

			src.x += offset.x;
		} // for

		src.x = 0;
		src.y += offset.y;
		if (colorChange == 0) colorChange = 1;
		else colorChange = 0;
	} // for

	DeleteObject(hBrush[0]);
	DeleteObject(hBrush[1]);
	DeleteObject(hBrush[2]);
	DeleteObject(hPen);
	SelectObject(hdc, hOldBrush);
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldFont);
	SetTextColor(hdc, colorOLD);
	SetBkMode(hdc, oldBackgroundMode);
}

void endTimer() {
	KillTimer(NULL, MyTimer);
	if (startTime_Min == COUNTTIME_MIN + (sudokuOrder.size - 2) * (sudokuOrder.size - 2))// + (sudokuOrder.size - 2)*(sudokuOrder.size - 2)
		DialogBox(hInst, MAKEINTRESOURCE(IDD_sudokuRank), hwnd, sudokuRank);
	thePlayer.correctNum = 0; thePlayer.Name = "", thePlayer.Date = "";
}



void CALLBACK refreshTime(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{
	RECT rc;
	rc = { WINDOWX - 100, 0, WINDOWX - 50, 50 };
	//
	InvalidateRect(hwnd, &rc, FALSE);
	startTime_Sec += 1;
	if (startTime_Sec == 60) {
		startTime_Min += 1;
		startTime_Sec = 0;
	}

	if (startTime_Min == COUNTTIME_MIN + (sudokuOrder.size - 2) * (sudokuOrder.size - 2)) { // 游戏时间到 2 + (sudokuOrder.size - 2)*(sudokuOrder.size - 2)


		time_t now;


		struct tm now_tm;
		time(&now);
		localtime_s(&now_tm, &now);

		thePlayer.type = sudokuOrder.data.Size();

		char buffer[256];
		strftime(buffer, 64, "%Y-%m-%d %I:%M%p", &now_tm);
		thePlayer.Date = buffer;
		// ctime_s(buffer, sizeof(buffer), &now);

		endTimer();

	}
	/*
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);
	DrawTime( ps, NULL);
	*/
}

void DrawTime(PAINTSTRUCT ps, HDC hdc) {

	//static PAINTSTRUCT ps = pS;
	//static HDC hdc = hdC;
	//HPEN hPen, hOldPen;
	//hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	//hOldPen = (HPEN)SelectObject(hdc, hPen);
	HFONT hFont = MyCreateFont(20, CHINESEBIG5_CHARSET, TEXT("标楷体")), hOldFont;
	hOldFont = (HFONT)SelectObject(hdc, hFont);
	int oldBackgroundMode = SetBkMode(hdc, TRANSPARENT);

	TCHAR theTime[256], theCorrectNum[256];
	RECT rc;
	wsprintf(theTime, TEXT("剩余时间: %d:%d"), 2 + (sudokuOrder.size - 2) * (sudokuOrder.size - 2) - startTime_Min, COUNTTIME_SEC - startTime_Sec);
	wsprintf(theCorrectNum, TEXT("答对题数: %d 题"), thePlayer.correctNum);
	rc = { WINDOWX - 230, 5, WINDOWX - 50, 50 };
	DrawText(hdc, theTime, -1, &rc, DT_LEFT);
	rc = { WINDOWX - 230, 30, WINDOWX - 50, 50 };
	DrawText(hdc, theCorrectNum, -1, &rc, DT_LEFT);

	SelectObject(hdc, hOldFont);
	SetBkMode(hdc, oldBackgroundMode);
} // 

char* convertTcharToChar(wchar_t* temp) {
	int dwNum;

	dwNum = MultiByteToWideChar(CP_ACP, 0, NULL, -1, (wchar_t*)temp, 0);
	//dwNum = dwNum*sizeof(wchar_t);
	char* pwText;
	pwText = new char[dwNum + 1];
	MultiByteToWideChar(CP_ACP, 0, (char*)pwText, -1, temp, dwNum + 1);

	return pwText;
	//LPWSTR ptr = wtext;
	//delete[]pwText;

}

void DrawRank(PAINTSTRUCT ps, HDC hdc) {
	TCHAR temp[256];
	HFONT hFont = MyCreateFont(20, CHINESEBIG5_CHARSET, TEXT("标楷体")), hOldFont;
	hOldFont = (HFONT)SelectObject(hdc, hFont);

	int oldBackgroundMode = SetBkMode(hdc, TRANSPARENT);
	RECT rc;

	//wsprintf(buffer, TEXT("%d"), rankList.length());
	//MessageBox( hwnd, buffer, L"", MB_OK );
	HPEN hPen, hOldPen;
	hPen = CreatePen(PS_SOLID, 3, RGB(72, 72, 147));

	hOldPen = (HPEN)SelectObject(hdc, hPen);

	rc = { 0, 0, 100, 20 };
	DrawText(hdc, L"名称", -1, &rc, DT_CENTER);

	rc = { 110, 0, 200, 20 };
	DrawText(hdc, L"题目类型", -1, &rc, DT_LEFT);

	rc = { 210, 0, 290, 20 };
	DrawText(hdc, L"答对题数", -1, &rc, DT_LEFT);

	rc = { 380, 0, 500, 20 };
	DrawText(hdc, L"日期", -1, &rc, DT_LEFT);


	MoveToEx(hdc, 0, 25, NULL);
	LineTo(hdc, 552, 25);

	const size_t newsize = 200;
	for (int i = 0; i < rankList.length(); i++) {

		MoveToEx(hdc, 100, 0, NULL); LineTo(hdc, 100, 600);
		rc = { 0, (i + 1) * 30, 100, (i + 1) * 30 + 20 };
		wsprintf(temp, L"%S", rankList.list[i].Name.c_str());
		DrawText(hdc, temp, -1, &rc, DT_CENTER);

		MoveToEx(hdc, 200, 0, NULL);
		LineTo(hdc, 200, 600);
		rc = { 100, (i + 1) * 30, 200, (i + 1) * 30 + 20 };

		wsprintf(temp, TEXT("%d x %d"), rankList.list[i].type, rankList.list[i].type);
		DrawText(hdc, temp, -1, &rc, DT_CENTER);

		MoveToEx(hdc, 310, 0, NULL);
		LineTo(hdc, 310, 600);
		rc = { 200, (i + 1) * 30, 290, (i + 1) * 30 + 20 };
		wsprintf(temp, TEXT("%d题"), rankList.list[i].correctNum);
		DrawText(hdc, temp, -1, &rc, DT_CENTER);

		MoveToEx(hdc, 552, 0, NULL);
		LineTo(hdc, 552, 600);
		rc = { 320, (i + 1) * 30, 550, (i + 1) * 30 + 20 };
		wsprintf(temp, L"%S", rankList.list[i].Date.c_str());
		DrawText(hdc, temp, -1, &rc, DT_CENTER);


		MoveToEx(hdc, 0, (i + 1) * 30 + 25, NULL);
		LineTo(hdc, 552, (i + 1) * 30 + 25);
	}

	SelectObject(hdc, hOldFont);
	SelectObject(hdc, hOldPen);
	SetBkMode(hdc, oldBackgroundMode);
}

void DrawTutorial(PAINTSTRUCT ps, HDC hdc) {

	HFONT hFont = MyCreateFont(20, CHINESEBIG5_CHARSET, TEXT("标楷体")), hOldFont;
	hOldFont = (HFONT)SelectObject(hdc, hFont);
	int oldBackgroundMode = SetBkMode(hdc, TRANSPARENT);
	TCHAR theNo[256];
	wsprintf(theNo, L"%d x %d 第%d题\n\n", sudokuOrder.size * sudokuOrder.size, sudokuOrder.size * sudokuOrder.size, sudokuOrder.no);
	TCHAR theTutorial[256] = L"操作:\n  用滑鼠点击空白区块\n  用键盘填入数字\n  0 为清空答案\n上下左右移动所选区块";
	lstrcat(theNo, theTutorial);
	RECT rc;

	rc = { WINDOWX - 230, 270, WINDOWX - 20, 500 };
	DrawText(hdc, theNo, -1, &rc, DT_LEFT);

	SelectObject(hdc, hOldFont);
	SetBkMode(hdc, oldBackgroundMode);
} // 

/*************************************************************************************/

HFONT MyCreateFont(int nHeight, DWORD dwCharSet, LPCTSTR lpName) {
	return (CreateFont(nHeight, 0, 0, 0,
		FW_DONTCARE,
		FALSE, FALSE, FALSE,
		dwCharSet,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		lpName));
}
/************************************** Slove ***********************************************/

void goBackTo(char* order) {
	if (strcmp(order, "sloveAll") == 0) {
		goBack->allowExcute = false;
		inSlove1 = false, inSlove2 = false;
		slove1->allowExcute = true, slove2->allowExcute = true;

		slove1_Add->allowExcute = false;
		slove1_Show->allowExcute = false;
		slove1_Delete->allowExcute = false;

		slove2_Practice->allowExcute = false;
		slove2_Play->allowExcute = false;
		slove2_Rank->allowExcute = false;


	}
	else if (strcmp(order, "slove1") == 0) {
		showSudoku = false;
		slove1_Add->allowExcute = true;
		slove1_Show->allowExcute = true;
		slove1_Delete->allowExcute = true;
		inSlove1Show = false;
	}
	else if (strcmp(order, "slove2") == 0) {
		showSudoku = false;
		slove2_Practice->allowExcute = true;
		slove2_Play->allowExcute = true;
		slove2_Rank->allowExcute = true;

		slove2_Send->allowExcute = false;
		inSlove1Show = false;
		startPlay = false;
		showRank = false;
		needTimer = false;
		startTime_Min = 0;
		startTime_Sec = 0;
		endTimer();
		sudoku.deleteData();

	}

	InvalidateRect(hwnd, NULL, TRUE);

}

/************************************** Slove1 ***********************************************/
void setShowSudoku(char* order) {

	if (strcmp(order, "play") == 0) {

		slove1_Add->allowExcute = false;
		slove1_Show->allowExcute = false;
		slove1_Delete->allowExcute = false;

		slove2_Practice->allowExcute = false;
		slove2_Play->allowExcute = false;
		slove2_Rank->allowExcute = false;

		showSudoku = true;
		startPlay = true;
		slove2_Send->allowExcute = true;
	}
	else if (strcmp(order, "show") == 0) {

		showSudoku = true;
		slove1_Add->allowExcute = false;
		slove1_Show->allowExcute = false;
		slove1_Delete->allowExcute = false;

		slove2_Practice->allowExcute = false;
		slove2_Play->allowExcute = false;
		slove2_Rank->allowExcute = false;
	}
	else if (strcmp(order, "rank") == 0) {

		showRank = true;
		slove1_Add->allowExcute = false;
		slove1_Show->allowExcute = false;
		slove1_Delete->allowExcute = false;

		slove2_Practice->allowExcute = false;
		slove2_Play->allowExcute = false;
		slove2_Rank->allowExcute = false;

	}
}


void slove1All() { // 显示任务一内容


	InvalidateRect(hwnd, NULL, TRUE);
	//MessageBox( NULL, L"", L"", MB_OK );

	inSlove1 = true;

	slove1->allowExcute = false;
	slove2->allowExcute = false;
	goBack->allowExcute = true;

	slove1_Add->allowExcute = true;
	slove1_Show->allowExcute = true;
	slove1_Delete->allowExcute = true;

	slove2_Practice->allowExcute = false;
	slove2_Play->allowExcute = false;
	slove2_Rank->allowExcute = false;
}

void slove1Add() {

	DialogBox(hInst, MAKEINTRESOURCE(IDD_sudokuMaker), hwnd, sudokuMaker);
	//MessageBox(NULL, L"a", L"", MB_OK);
}

void slove1Show() {
	// MessageBox(NULL, L"s", L"", MB_OK);
	//inSlove1Show = true;
	goBack->allowExcute = true;
	inSlove1Show = true;
	inSlove1Delete = false;
	DialogBox(hInst, MAKEINTRESOURCE(IDD_showQuestion), hwnd, sudokuManeger);

}

void slove1Delete() {
	// MessageBox(NULL, L"d", L"", MB_OK);
	inSlove1Delete = true;
	inSlove1Show = false;
	DialogBox(hInst, MAKEINTRESOURCE(IDD_deleteQuestion), hwnd, sudokuManeger);

}

/************************************** Slove2 ***********************************************/
void slove2All() { // 显示任务二内容


	InvalidateRect(hwnd, NULL, TRUE);
	//MessageBox( NULL, L"slove2", L"", MB_OK );

	inSlove2 = true;

	goBack->allowExcute = true;

	slove1->allowExcute = false;
	slove2->allowExcute = false;

	slove2_Send->allowExcute = false;

	slove1->allowExcute = false;
	slove1_Add->allowExcute = false;
	slove1_Show->allowExcute = false;
	slove1_Delete->allowExcute = false;

	slove2_Practice->allowExcute = true;
	slove2_Play->allowExcute = true;
	slove2_Rank->allowExcute = true;

}

void slove2Play() {
	//MessageBox(NULL, L"p", L"", MB_OK);
	needTimer = true;
	DialogBox(hInst, MAKEINTRESOURCE(IDD_sudokuPlay), hwnd, sudokuPlay);
	//DialogBox(hInst, MAKEINTRESOURCE(IDD_showQuestion), hwnd, sudokuManeger);
} // slove2

void slove2Practice() {
	DialogBox(hInst, MAKEINTRESOURCE(IDD_sudokuPlay), hwnd, sudokuPlay);
} // slove2

void slove2Rank() {
	//MessageBox(NULL, L"r", L"", MB_OK);
	//DialogBox(hInst, MAKEINTRESOURCE(IDD_showQuestion), hwnd, sudokuRank);

	rankList.ReadRankList();
	rankList.SortRankList();
	rankList.SaveRankList();


	showRank = true;
	slove2_Practice->allowExcute = false;
	slove2_Play->allowExcute = false;
	slove2_Rank->allowExcute = true;
	InvalidateRect(hwnd, NULL, TRUE);
} // slove2

bool slove2Send() {
	TCHAR buffer[256];

	int dSize = sudoku.Size() * sudoku.Size();


	for (int i = 0; i < dSize; i++) {
		if (sudoku.Sudoku()[i].isRepeat(sudoku.Sudoku(), dSize, sudoku.Sudoku()[i].num)) {
			//wsprintf(buffer, TEXT("index: %d Num: %d"), i, sudoku.Sudoku()[i].num);
			//MessageBox(hwnd, buffer, L"", MB_OK);
			MessageBox(hwnd, L"答案错误", L"", MB_OK);
			return false;
		} // if
	} // for
	// Correct!!


	//delete [] order.data.Sudoku();
	//orderSudoku orderl;

	srand(time(NULL));
	sudokuOrder.order = 2, sudokuOrder.no = 1 + rand() % (sudokuOrder.numOfQuestion[sudokuOrder.size]);

	sudokuIO(&sudokuOrder);
	//wsprintf(buffer, TEXT("Size: %d"), order.size);
	//MessageBox(hwnd, buffer, L"", MB_OK);
	//checkError(hwnd);
	sudoku = sudokuOrder.data;

	sudoku.convertToNoAnswer();
	thePlayer.correctNum++;
	return true;
}

/************************************** Dialog ***********************************************/
TCHAR szTypeList[9][10] = { TEXT("4*4"), TEXT("9*9"), TEXT("16*16"), TEXT("25*25"), TEXT("36*36"), TEXT("49*49"), TEXT("64*64"), TEXT("81*81"), TEXT("100*100") };
INT_PTR CALLBACK sudokuMaker(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hmakeList, hsumOfAdd;
	int sumOfAdd = 1;
	static TCHAR buffer[256];
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		hmakeList = GetDlgItem(hDlg, DLG_slove1MakeList);
		hsumOfAdd = GetDlgItem(hDlg, DLG_slove1SumOfAdd);
		/*************************************************/
		for (int i = 0; i < NUMOFTYPE; i++) {
			ComboBox_AddString(hmakeList, szTypeList[i]);
		} // for

		ComboBox_SetCurSel(hmakeList, 0);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case IDOK:
			Edit_GetText(hsumOfAdd, buffer, 256);
			for (int i = 0; i < lstrlen(buffer); i++) if ('9' < buffer[i] || buffer[i] < '0') return (INT_PTR)TRUE;
			sumOfAdd = _wtoi(buffer);
			if (sumOfAdd > 1000) {
				MessageBox(hDlg, L"请输入1000内的题数", L"Error", MB_OK);
				return (INT_PTR)TRUE;
			} // if
			else if (lstrcmp(buffer, L"") == 0) sumOfAdd = 1;
			sudokuOrder.order = 1, sudokuOrder.size = (ComboBox_GetCurSel(hmakeList) + 2), sudokuOrder.sumOfAdd = sumOfAdd;
			if (sudokuIO(&sudokuOrder) == -1)
				MessageBox(hwnd, L"已无新题型", L"", MB_OK);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case DLG_slove1MakeList:


			return (INT_PTR)TRUE;
		} // switch

		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK sudokuManeger(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int intType, intNo, intNoDes, intNum;
	static HWND hOk, hType, hNo, hNoDes, hNum;
	//static HWND hDeleteType, hDeleteNo;
	static int totalQuestion, choosedNumber, temp;
	static LPCWSTR strNum;
	static TCHAR buffer[256], str[256];
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:

		if (inSlove1Show) {
			intType = DLG_slove1ShowType;
			intNo = DLG_slove1questionNo;
			intNum = DLG_slove1QuestionNum;
		} // if
		else if (inSlove1Delete) {
			intType = DLG_deleteSize;
			intNo = DLG_deleteNoSrc;
			intNum = DLG_deleteNumOfType;
		} // else if

		hType = GetDlgItem(hDlg, intType);
		hNo = GetDlgItem(hDlg, intNo);
		hNoDes = GetDlgItem(hDlg, DLG_deleteNoDes);
		hNum = GetDlgItem(hDlg, intNum);
		hOk = GetDlgItem(hDlg, IDOK);
		/*************************************************/
		for (int i = 0; i < NUMOFTYPE; i++) {
			ComboBox_AddString(hType, szTypeList[i]);
		} // for

		ComboBox_SetCurSel(hType, 0);
		/*************************************************/
		sudokuOrder.order = 0;
		sudokuIO(&sudokuOrder);
		wsprintf(buffer, TEXT("选择题数: (目前共%d题)"), sudokuOrder.numOfQuestion[0]);
		Edit_SetText(hNum, buffer);

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			Edit_GetText(hNo, buffer, 256);
			Edit_GetText(hNoDes, str, 256);
			sudokuOrder.size = (ComboBox_GetCurSel(hType) + 2);
			for (int i = 0; i < lstrlen(buffer); i++) if ('9' < buffer[i] || buffer[i] < '0') return (INT_PTR)TRUE;
			if (sudokuOrder.numOfQuestion[sudokuOrder.size - 2] < _wtoi(buffer) || _wtoi(buffer) < 1) {
				MessageBox(hDlg, L"请输入正确题号", L"Error", MB_OK);
				return (INT_PTR)TRUE;
			} // if

			if (inSlove1Show) {
				sudokuOrder.data.deleteData();
				sudokuOrder.order = 2, sudokuOrder.size = (ComboBox_GetCurSel(hType) + 2), sudokuOrder.no = _wtoi(buffer);
				sudokuIO(&sudokuOrder);
				sudoku = sudokuOrder.data;
				sudoku.convertToNoAnswer();
				//wsprintf(buffer, TEXT("Size: %d"), order.data.Size());
				//MessageBox(hDlg, buffer, L"", MB_OK);
				/***************************************************************/
				setShowSudoku("show");
				/***************************************************************/

			} // if
			else if (inSlove1Delete) {
				for (int i = 0; i < lstrlen(buffer); i++) if ('9' < buffer[i] || buffer[i] < '0') return (INT_PTR)TRUE;
				if (_wtoi(str) < _wtoi(buffer)) sudokuOrder.noDes = 0;
				else sudokuOrder.noDes = _wtoi(str) - _wtoi(buffer) + 1;
				if (_wtoi(str) > sudokuOrder.numOfQuestion[sudokuOrder.size - 2]) sudokuOrder.noDes = (sudokuOrder.numOfQuestion[sudokuOrder.size - 2] - _wtoi(buffer)) + 1;
				sudokuOrder.order = 3, sudokuOrder.size = (ComboBox_GetCurSel(hType) + 2), sudokuOrder.no = _wtoi(buffer);

				sudokuIO(&sudokuOrder);
				inSlove1Delete = false;
			} // else if

			EndDialog(hDlg, LOWORD(wParam));
			InvalidateRect(hwnd, NULL, FALSE);
			Button_Enable(hOk, false);
			return (INT_PTR)TRUE;
		case IDCANCEL:
			//sudokuIO(1, (ComboBox_GetCurSel(hmakeList) + 2));

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case DLG_deleteNoSrc:
			Edit_GetText(hNo, buffer, 256);
			Edit_SetText(hNoDes, buffer);
			return (INT_PTR)TRUE;
		case DLG_slove1ShowType:
		case DLG_deleteSize:
			sudokuOrder.size = (ComboBox_GetCurSel(hType) + 2);
			wsprintf(buffer, TEXT("选择题数: (目前共%d题)"), sudokuOrder.numOfQuestion[sudokuOrder.size - 2]);
			Edit_SetText(hNum, buffer);
			//一次不要加载太多题,跑完在加载,直到无题型
			return (INT_PTR)TRUE;
		} // switch

		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK sudokuPlay(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	static HWND hType, hNum, hStatic;
	TCHAR buffer[256];
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		hType = GetDlgItem(hDlg, DLG_slove2Type);
		hNum = GetDlgItem(hDlg, DLG_slove2PlayNum);
		hStatic = GetDlgItem(hDlg, DLG_slvoe2PlayStatic);
		for (int i = 0; i < NUMOFTYPE; i++) {
			ComboBox_AddString(hType, szTypeList[i]);
		} // for

		ComboBox_SetCurSel(hType, 0);
		if (needTimer) ShowWindow(hNum, SW_HIDE); // Practice Mode
		else {
			sudokuOrder.order = 0;
			sudokuIO(&sudokuOrder);
			wsprintf(buffer, TEXT("选择题数: (目前共%d题)"), sudokuOrder.numOfQuestion[0]);
			Edit_SetText(hStatic, buffer);
		} // else

		sudokuOrder.order = 0;
		sudokuIO(&sudokuOrder);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			// 取得此类型总题数
			sudokuOrder.order = 0; sudokuIO(&sudokuOrder);
			sudokuOrder.size = (ComboBox_GetCurSel(hType) + 2);
			if (needTimer && sudokuOrder.numOfQuestion[sudokuOrder.size - 2] < maxNumToAdd[sudokuOrder.size - 2]) {
				sudokuOrder.order = 1, sudokuOrder.sumOfAdd = maxNumToAdd[sudokuOrder.size - 2];
				if (sudokuIO(&sudokuOrder) == -1)
					MessageBox(hwnd, L"已无新题型", L"", MB_OK);
				sudokuOrder.order = 0; sudokuIO(&sudokuOrder);
			} // if

			//wsprintf(buffer, TEXT("Size: %d"), sudokuOrder.size);
			//MessageBox(hwnd, buffer, L"", MB_OK);
			if (sudokuOrder.numOfQuestion[sudokuOrder.size - 2] <= 0) {
				MessageBox(hwnd, L"尚未加入题型,请先新增题型", L"Error", MB_OK);
				return (INT_PTR)TRUE;
			} // if

			// 先取出同类型的所有题目
			//sudokuList.data = new dataSudoku[totalQuestion];
			//sudokuList.size = totalQuestion;


			srand(time(NULL));
			sudokuOrder.order = 2, sudokuOrder.size = (ComboBox_GetCurSel(hType) + 2), sudokuOrder.no = 1 + rand() % sudokuOrder.numOfQuestion[sudokuOrder.size - 2];

			if (!needTimer) {
				Edit_GetText(hNum, buffer, 256);
				for (int i = 0; i < lstrlen(buffer); i++) if ('9' < buffer[i] || buffer[i] < '0') return (INT_PTR)TRUE;
				if (sudokuOrder.numOfQuestion[sudokuOrder.size - 2] < _wtoi(buffer) || _wtoi(buffer) < 1) {
					MessageBox(hDlg, L"请输入正确题号", L"Error", MB_OK);
					return (INT_PTR)TRUE;
				} // if

				sudokuOrder.no = _wtoi(buffer);
			} // if

			sudokuIO(&sudokuOrder);
			sudoku.deleteData();
			sudoku = sudokuOrder.data;
			//wsprintf(buffer, TEXT("Size: %d"), sudokuOrder.size);
			//MessageBox(hwnd, buffer, L"", MB_OK);

			sudoku.convertToNoAnswer();

			// SetWindowPos(hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
			setShowSudoku("play");
			EndDialog(hDlg, LOWORD(wParam));
			InvalidateRect(hwnd, NULL, FALSE);
			// start = clock();
			if (needTimer)
				MyTimer = SetTimer(NULL, MYTIMER, 1000, (TIMERPROC)& refreshTime);
			return (INT_PTR)TRUE;
		case DLG_slove2Type:
			if (needTimer) return (INT_PTR)TRUE;
			sudokuOrder.size = (ComboBox_GetCurSel(hType) + 2);
			wsprintf(buffer, TEXT("选择题型: (目前共%d题)"), sudokuOrder.numOfQuestion[sudokuOrder.size - 2]);
			Edit_SetText(hStatic, buffer);
			return (INT_PTR)TRUE;
		case IDCANCEL:
			needTimer = false;
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		} // switch

		break;
	}

	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK sudokuRank(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	static HWND hType, hCorrectNum, hPlayerName;
	TCHAR buffer[128];
	char temp[256];
	string check = "";
	int find = 0;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		startTime_Min = 0;
		startTime_Sec = 0;
		hType = GetDlgItem(hDlg, DLG_slove2RankType);
		hCorrectNum = GetDlgItem(hDlg, DLG_slove2RankCorrect);
		hPlayerName = GetDlgItem(hDlg, DLG_slove2PlayerName);
		wsprintf(buffer, TEXT("类型: %d X %d"), thePlayer.type, thePlayer.type);
		Edit_SetText(hType, buffer);
		wsprintf(buffer, TEXT("答对题数: %d题"), thePlayer.correctNum);
		Edit_SetText(hCorrectNum, buffer);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			Edit_GetText(hPlayerName, buffer, 20);
			if (lstrlen(buffer) <= 0) {
				MessageBox(hDlg, L"请输入玩家名称", L"Error", MB_OK);
				return (INT_PTR)TRUE;
			} // if
			else if (lstrlen(buffer) > 10) {
				MessageBox(hDlg, L"请输10字以内名称", L"Error", MB_OK);
				return (INT_PTR)TRUE;
			} // if

			//
			sprintf_s(temp, "%S", buffer);
			for (int i = 0; i < strlen(temp); i++)	if (temp[i] == ' ') find = -1;
			if (strlen(temp) <= 0 || find < 0) {
				MessageBox(hDlg, L"请误输入中文或空白字元", L"Error", MB_OK);
				return (INT_PTR)TRUE;
			} // if
			thePlayer.Name = temp;
			rankList.ReadRankList();

			rankList.add(thePlayer);
			rankList.SortRankList();
			rankList.SaveRankList();
			EndDialog(hDlg, LOWORD(wParam));
			goBackTo("slove2");
			slove2Rank();
			return (INT_PTR)TRUE;
		} // switch

		break;
	}

	return (INT_PTR)FALSE;
}

void showAbout() {

	hAbout = CreateDialog(hInst, MAKEINTRESOURCE(IDD_ABOUT), NULL, NULL);
	ShowWindow(hAbout, SW_SHOW);
	SetWindowPos(hAbout, HWND_TOPMOST, GetSystemMetrics(SM_CXSCREEN) / 2 - 100, GetSystemMetrics(SM_CYSCREEN) / 2 - 100, 0, 0, SWP_NOSIZE);

}