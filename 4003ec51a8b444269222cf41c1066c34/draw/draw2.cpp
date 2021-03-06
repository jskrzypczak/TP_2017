// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw2.h"
#include "elevator.h"
#include <vector>
#include <cstdio>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100
#define TMR_1 1
#define TMR_2 2
#define MAX_FLOOR 3

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name




// buttons
HWND hwndButton;
HWND hText;
HWND hUpDown;
HBITMAP bkground;
HBITMAP men;
HDC memHdc;

CElevator elevator(MAX_FLOOR);
enum TFloor elevatorFloor = first;
enum TFloor newFloor = first;
uint16_t elevatorY = elevatorFloor;
bool onFloor = true;
elev_out_t elevatorStatus;
vector2D_t floorMatrix, floorMatrix2;
static bool once = true;
int elevatorPeople = 0;
RECT drawArea = { 280, 0, 1100, 800 };
wchar_t weightText, amountText;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK		DlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);

void DrawDoubleBuffer(HWND hWnd);

bool CALLBACK SetFont(HWND child, LPARAM font) {
	SendMessage(child, WM_SETFONT, font, true);
	return true;
}

void InitFloorMatrix(unsigned int floorAmount);

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{

	if (drawArea == NULL)
		InvalidateRect(hWnd, NULL, FALSE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, FALSE); //repaint drawArea

	DrawDoubleBuffer(hWnd);

}


// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	// TODO: Place code here.
	InitFloorMatrix(MAX_FLOOR);
	MSG msg;
	HACCEL hAccelTable;



	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
void InsertNewMan(unsigned int startFloor, unsigned int endFloor);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance; // Store instance handle (of exe) in our global variable

	// main window
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, 0, 1200, 800, NULL, NULL, hInstance, NULL);

	bkground = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
	men = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	// create button and store the handle    

	hwndButton = CreateWindow(TEXT("button"),                     // The class name required is button
		TEXT("1"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		250, 530,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_GROUND_FIRST,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                     // The class name required is button
		TEXT("2"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		250, 500,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_GROUND_SECOND,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                     // The class name required is button
		TEXT("0"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		1100, 325,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_FIRST_GROUND,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                     // The class name required is button
		TEXT("2"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		1100, 295,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_FIRST_SECOND,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                     // The class name required is button
		TEXT("1"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		250, 100,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_SECOND_FIRST,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                     // The class name required is button
		TEXT("0"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		250, 130,                                  // the left and top co-ordinates
		25, 25,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_SECOND_GROUND,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);


	hText = CreateWindowW(L"Static", L"Ilo�� os�b:",
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		70, 20, 70, 30,
		hWnd, (HMENU)1, NULL, NULL);

	 hText = CreateWindowW(L"Static", L"WAGA:", 
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                85, 50, 70, 30, 
                hWnd, (HMENU) 1, NULL, NULL);

	 hText = CreateWindowW(L"Static", L"Ilo�� os�b:",
		 WS_CHILD | WS_VISIBLE | SS_LEFT,
		 130, 20, 70, 30,
		 hWnd, (HMENU)ID_AMOUNT, NULL, NULL);

	 hText = CreateWindowW(L"Static", L"WAGA:",
		 WS_CHILD | WS_VISIBLE | SS_LEFT,
		 130, 50, 70, 30,
		 hWnd, (HMENU)ID_WEIGHT, NULL, NULL);
            

	if (!hWnd)
	{
		return FALSE;
	}
	InsertNewMan(0, 1);
	//elevator.load_array(floorMatrix);
	elevatorStatus = elevator.make_turn();
	//elevator.make_order();
	floorMatrix = elevatorStatus.floor_array_next;
	floorMatrix2 = elevatorStatus.floor_array_prev;
	SetTimer(hWnd, TMR_1, 1, NULL);
	SetTimer(hWnd, TMR_2, 500, NULL);

	EnumChildWindows(hWnd, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));

	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window (low priority)
//  WM_DESTROY	- post a quit message and return
//
// 175 h
// 209 w


void DrawMenInElevator(HDC hdc, int xOffset)
{
	HDC menHdc;
	menHdc = CreateCompatibleDC(hdc);
	SelectObject(menHdc, men);

	BitBlt(hdc, 600+ xOffset, elevatorY+60, 820, 800, menHdc, 0, 0, SRCCOPY);

	DeleteDC(menHdc);
}

// 3 pietro: 587,16
// 2 pietro: 587,216
// 1 pietro: 587,416
void DrawElevator(HDC hdc)
{
	Font font(&FontFamily(L"Arial"), 12);

	LinearGradientBrush brush(Rect(0, 0, 100, 100), Color::Red, Color::Yellow, LinearGradientModeHorizontal);

	Graphics graphics(hdc);
	Pen pen2(Color(255, 0, 255, 0), 4.5f);
	graphics.DrawRectangle(&pen2, 587, elevatorY, 198, 169);
	graphics.DrawString(L"TEST", -1, &font, PointF(0, 0), &brush);
	int people = elevatorStatus.elev_content[0] + elevatorStatus.elev_content[1] + elevatorStatus.elev_content[2];
	switch (elevatorPeople)
	{
	case 0:
		
		break;
	case 1:
		DrawMenInElevator(hdc,0);
		break;
	case 2:
		DrawMenInElevator(hdc, 0);
		DrawMenInElevator(hdc, 60);
		break;
	case 3:
	default:
		DrawMenInElevator(hdc, 0);
		DrawMenInElevator(hdc, 60);
		DrawMenInElevator(hdc, 120);
		break;
	}
}

Point GetPosForMen(TFloor elevatorFloor)
{
	Point retPoint;
	switch (elevatorFloor)
	{
	case ground:
		retPoint.X = 520;
		retPoint.Y = 480;
		break;
	case first:
		retPoint.X = 800;
		retPoint.Y = 280;
		break;
	case second:
		retPoint.X = 520;
		retPoint.Y = 80;
		break;
	default:
		break;
	}
	return retPoint;
}
Point GetPosForMen(unsigned int floor)
{
	Point retPoint;
	switch (floor)
	{
	case 0:
		retPoint.X = 570;
		retPoint.Y = 480;
		break;
	case 1:
		retPoint.X = 750;
		retPoint.Y = 280;
		break;
	case 2:
		retPoint.X = 570;
		retPoint.Y = 80;
		break;
	default:
		break;
	}
	return retPoint;
}

void DrawMen(HDC hdc)
{
	HDC menHdc;
	menHdc = CreateCompatibleDC(hdc);
	SelectObject(menHdc, men);
	for (int floor = 0; floor < MAX_FLOOR; floor++)
	{
		int amount = floorMatrix2[floor][0] + floorMatrix2[floor][1] + floorMatrix2[floor][2];
		for (; amount > 0; amount--)
		{
			if (floor == 1)
				BitBlt(hdc, GetPosForMen(floor).X + 56 * amount - 1, GetPosForMen(floor).Y, 820, 800, menHdc, 0, 0, SRCCOPY);
			else
				BitBlt(hdc, GetPosForMen(floor).X - 56 * amount - 1, GetPosForMen(floor).Y, 820, 800, menHdc, 0, 0, SRCCOPY);
		}
	}

	
	DeleteDC(menHdc);
}

void DrawDoubleBuffer(HWND hWnd)
{
	RECT Client_Rect;
	PAINTSTRUCT ps;
	HDC  bkgHdc;
	HDC hdc;
	HBITMAP memBitmap;
	GetClientRect(hWnd, &Client_Rect);
	int win_width = Client_Rect.right - Client_Rect.left;
	int win_height = Client_Rect.bottom + Client_Rect.left;
	hdc = BeginPaint(hWnd, &ps);
	memHdc = CreateCompatibleDC(hdc);
	bkgHdc = CreateCompatibleDC(hdc);

	memBitmap = CreateCompatibleBitmap(hdc, win_width, win_height);


	SelectObject(memHdc, memBitmap);
	SelectObject(bkgHdc, bkground);

	FillRect(memHdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));

	BitBlt(memHdc, 280, 0, 820, 800, bkgHdc, 0, 0, SRCCOPY);

	DrawElevator(memHdc);
	DrawMen(memHdc);
	BitBlt(hdc, 0, 0, win_width, win_height, memHdc, 0, 0, SRCCOPY);





	DeleteObject(memBitmap);
	DeleteDC(bkgHdc);
	DeleteDC(memHdc);

	DeleteDC(hdc);
	EndPaint(hWnd, &ps);
}

bool AnimateElevator(TFloor &desiredFloor)
{


	if (elevatorFloor > elevatorY)
	{
		elevatorY++;
		return true;
	}
	else if (elevatorFloor < elevatorY)
	{
		elevatorY--;
		return true;
	}
	else
	{

		elevatorFloor = desiredFloor;
		return false;
	}
}

void InitFloorMatrix(unsigned int floorAmount)
{
	floorMatrix.resize(floorAmount, vector<int>(floorAmount));
	floorMatrix2.resize(floorAmount, vector<int>(floorAmount));
}

void InsertNewMan(unsigned int startFloor, unsigned int endFloor)
{
	floorMatrix[startFloor][endFloor]++;
	floorMatrix2[startFloor][endFloor]++;
}
void DebugMatrix()
{
	DrawMen(memHdc);
	system("cls");
	cout << "Macierz do Ciebie:" << endl;
	for (auto line : floorMatrix)
	{
		for (auto col : line)
		{
			cout << " " << col;
		}
		cout << endl;
	}
	cout << "Macierz do rysowania:" << endl;
	for (auto line : floorMatrix2)
	{
		for (auto col : line)
		{
			cout << " " << col;
		}
		cout << endl;
	}
	cout << "Zawartosc windy:" << endl;
	for (auto line : elevatorStatus.elev_content)
	{
		cout << " " << line;
	}
	
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_GROUND_FIRST:
			newFloor = ground;
			InsertNewMan(0, 1);
			elevator.load_array(floorMatrix);
			once = true;
			DebugMatrix();
			repaintWindow(hWnd, hdc, ps, &drawArea);
			break;
		case ID_GROUND_SECOND:
			newFloor = ground;
			InsertNewMan(0, 2);
			elevator.load_array(floorMatrix);
			once = true;
			floorMatrix2 = floorMatrix;
			DebugMatrix();
			repaintWindow(hWnd, hdc, ps, &drawArea);
			break;
		case ID_FIRST_SECOND:
			newFloor = first;
			InsertNewMan(1, 2);
			elevator.load_array(floorMatrix);
			once = true;
			floorMatrix2 = floorMatrix;
			DebugMatrix();
			repaintWindow(hWnd, hdc, ps, &drawArea);
			break;
		case ID_FIRST_GROUND:
			newFloor = first;
			InsertNewMan(1, 0);
			elevator.load_array(floorMatrix);
			once = true;
			floorMatrix2 = floorMatrix;
			DebugMatrix();
			repaintWindow(hWnd, hdc, ps, &drawArea);
			break;
		case ID_SECOND_FIRST:
			newFloor = second;
			InsertNewMan(2, 1);
			elevator.load_array(floorMatrix);
			once = true;
			floorMatrix2 = floorMatrix;
			DebugMatrix();
			repaintWindow(hWnd, hdc, ps, &drawArea);
			break;
		case ID_SECOND_GROUND:
			newFloor = second;
			InsertNewMan(2, 0);
			elevator.load_array(floorMatrix);
			once = true;
			floorMatrix2 = floorMatrix;
			DebugMatrix();
			repaintWindow(hWnd, hdc, ps, &drawArea);
			break;
		case ID_ZOOM_OUT:
			newFloor = first;
			repaintWindow(hWnd, hdc, ps, &drawArea);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));
		repaintWindow(hWnd, hdc, ps, &drawArea);
		EndPaint(hWnd, &ps);
		break;
	case WM_ERASEBKGND:
		return true;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CREATE:
		EnumChildWindows(hWnd, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));
		break;


	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			if (AnimateElevator(newFloor))
			{

				repaintWindow(hWnd, hdc, ps, &drawArea);
				onFloor = false;

			}
			else
			{
				onFloor = true;
				//once = true;
				if (onFloor && once)
				{

					DebugMatrix();
					SetDlgItemInt(hWnd, ID_WEIGHT, elevatorStatus.load, false);
					SetDlgItemInt(hWnd, ID_AMOUNT, elevatorStatus.people_elev, false);
					Sleep(1000);
					
					floorMatrix2 = floorMatrix;
					floorMatrix = elevatorStatus.floor_array_next;
					//floorMatrix2 = elevatorStatus.floor_array_prev;
					elevatorPeople = elevatorStatus.people_elev;
					elevatorStatus = elevator.make_turn();
					newFloor = elevatorStatus.next_floor;
					once = false;

					
					
				}
			}

			
	
			break;
		case TMR_2:
			if (!once && !onFloor) once = true;


			

			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}




BOOL CALLBACK DlgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	TCHAR buffer[60];

	switch (Msg)
	{
	case WM_COMMAND:
	{

		// reakcja na przyciski
		switch (LOWORD(wParam))
		{
		case ID_YES: EndDialog(hwnd, ID_YES); break;
		case ID_NO: EndDialog(hwnd, ID_NO); break;
		}
	}
	break;

	default: return FALSE;
	}

	return TRUE;
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
