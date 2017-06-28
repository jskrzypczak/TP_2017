// Projekt winda.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Projekt winda.h"
#include <deque>

#define MAX_LOADSTRING 100

#define ID_BUTTON_F4_0 0
#define ID_BUTTON_F4_1 1
#define ID_BUTTON_F4_2 2
#define ID_BUTTON_F4_3 3
#define ID_BUTTON_F4_4 4

#define ID_BUTTON_F3_0 5
#define ID_BUTTON_F3_1 6
#define ID_BUTTON_F3_2 7
#define ID_BUTTON_F3_3 8
#define ID_BUTTON_F3_4 9

#define ID_BUTTON_F2_0 10
#define ID_BUTTON_F2_1 11
#define ID_BUTTON_F2_2 12
#define ID_BUTTON_F2_3 13
#define ID_BUTTON_F2_4 14

#define ID_BUTTON_F1_0 15
#define ID_BUTTON_F1_1 16
#define ID_BUTTON_F1_2 17
#define ID_BUTTON_F1_3 18
#define ID_BUTTON_F1_4 19

#define ID_BUTTON_F0_0 20
#define ID_BUTTON_F0_1 21
#define ID_BUTTON_F0_2 22
#define ID_BUTTON_F0_3 23
#define ID_BUTTON_F0_4 24

#define TM_1 25
#define TM_2 26
#define TM_3 27


struct ORDER
{
	int to;
	int where;
	int direction;
	bool b_to;
	bool b_where;
};

struct HUMAN
{
	int floor;
	int direction;
	int pos_x;
	int pos_y;
	int time = 0;
	bool stop = false;
};

std::deque <ORDER> Order;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


int TIME_1 = 0;
int TIME_2 = 0;

int Hum_ID = 0;
int elev_pos;

bool ElevatorOpen = false;

const int margin = 6;

const int MAX_H = 50;

HUMAN HUMAN_AR[MAX_H];

#pragma region MainWindowDivision

const int Mainrect_x = 200;
const int Mainrect_y = 0;
const int Mainrect_height = 700;
const int Mainrect_width = 700;

const int Floor_height = 130;
const int Floor_width = 300;

const int Elevator_height = 665;
const int Elevator_width = 200;

const int Exit_height = Floor_height;
const int Exit_width = 170;

const int Human_height = 50;
const int Human_width = 12;

RECT MAINRECT = { Mainrect_x, Mainrect_y, Mainrect_x + Mainrect_width, Mainrect_y + Mainrect_height };

RECT FLOOR_4 = { Mainrect_x, Mainrect_y + Floor_height *0, Mainrect_x + Floor_width, Mainrect_y + Floor_height *1 };
RECT FLOOR_3 = { Mainrect_x, Mainrect_y + Floor_height *1, Mainrect_x + Floor_width, Mainrect_y + Floor_height *2 };
RECT FLOOR_2 = { Mainrect_x, Mainrect_y + Floor_height *2, Mainrect_x + Floor_width, Mainrect_y + Floor_height *3 };
RECT FLOOR_1 = { Mainrect_x, Mainrect_y + Floor_height *3, Mainrect_x + Floor_width, Mainrect_y + Floor_height *4 };
RECT FLOOR_0 = { Mainrect_x, Mainrect_y + Floor_height *4, Mainrect_x + Floor_width, Mainrect_y + Floor_height *5 };

RECT ELEVATOR = {	Mainrect_x + Floor_width, 
					Mainrect_y + margin + 1, 
					Mainrect_x + Floor_width + Elevator_width,
					Mainrect_y + Elevator_height - 1};

RECT EXIT_4 = {		Mainrect_x + Floor_width + Elevator_width,
					Mainrect_y + Exit_height * 0,
					Mainrect_x + Floor_width + Elevator_width + Exit_width,
					Mainrect_y + Exit_height * 1 };

RECT EXIT_3 = {		Mainrect_x + Floor_width + Elevator_width,
					Mainrect_y + Exit_height * 1,
					Mainrect_x + Floor_width + Elevator_width + Exit_width,
					Mainrect_y + Exit_height * 2 };

RECT EXIT_2 = {		Mainrect_x + Floor_width + Elevator_width,
					Mainrect_y + Exit_height * 2,
					Mainrect_x + Floor_width + Elevator_width + Exit_width,
					Mainrect_y + Exit_height * 3 };

RECT EXIT_1 = {		Mainrect_x + Floor_width + Elevator_width,
					Mainrect_y + Exit_height * 3,
					Mainrect_x + Floor_width + Elevator_width + Exit_width,
					Mainrect_y + Exit_height * 4 };

RECT EXIT_0 = {		Mainrect_x + Floor_width + Elevator_width,
					Mainrect_y + Exit_height * 4,
					Mainrect_x + Floor_width + Elevator_width + Exit_width,
					Mainrect_y + Exit_height * 5 };

#pragma endregion

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void Load(int to, int where, int dir, bool b_to, bool b_where)
{
	ORDER buffor_load;
	buffor_load.to = to;
	buffor_load.where = where;
	buffor_load.direction = dir;
	buffor_load.b_to = b_to;
	buffor_load.b_where = b_where;
	Order.push_back(buffor_load);
}

void CreateHuman(int flr, int drctn)
{
	HUMAN_AR[Hum_ID].floor = flr;
	HUMAN_AR[Hum_ID].direction = drctn;
	HUMAN_AR[Hum_ID].pos_x = Mainrect_x;
	HUMAN_AR[Hum_ID].pos_y = Mainrect_y + Floor_height * (5 - flr) - Human_height;
	Hum_ID++;
}

bool ElevatorFull(int p)
{
	int sum = 0;
	for (int m = 0; m < Hum_ID; m++)
	{
		if ((HUMAN_AR[Hum_ID].pos_x + Human_width) >(Mainrect_x + Floor_width + 3)) sum += Human_width;
	}
	if (sum >= Elevator_width)	return true;
	return false;
}

bool ElevatorAvaible(int p, int pos)
{
	if ((HUMAN_AR[p].pos_x + Human_width) == (Mainrect_x + Floor_width - 4))
	{
		if ((HUMAN_AR[p].floor == abs(((pos - 10 - Mainrect_y) / Floor_height) - 4)) && ElevatorOpen) return true;
		return false;
	}
	else if ((HUMAN_AR[p].pos_x + Human_width) == (Mainrect_x + Floor_width + Elevator_width - 10))	//right side of the shaft
	{
		if (HUMAN_AR[p].direction == HUMAN_AR[p].floor && ElevatorOpen) return true;
		return false;
	}
	return false;
}

bool WhenPeopleMeet(int p, int pos)
{
	if (ElevatorAvaible(p, pos) && (HUMAN_AR[p].direction == HUMAN_AR[Hum_ID].floor))	return false;
	for (int n = 0; n < Hum_ID; n++)
	{
		if (HUMAN_AR[p].pos_x + Human_width + 4 == HUMAN_AR[n].pos_x && HUMAN_AR[p].floor == HUMAN_AR[n].floor)	return true;
		else if (((HUMAN_AR[p].pos_x + Human_width) > HUMAN_AR[n].pos_x) && ((HUMAN_AR[p].pos_x + Human_width) < HUMAN_AR[n].pos_x + Human_width) && HUMAN_AR[p].floor == HUMAN_AR[n].floor) return true;
	}
	return false;
}
bool AnimationStop(int p, int pos)
{
	if ((HUMAN_AR[p].pos_x + Human_width) == (Mainrect_x + Floor_width - 4))		//lewa sciana windy, dodac inne warunki 
	{
		if (!ElevatorAvaible(p, pos))
		{
			HUMAN_AR[p].stop = true;
			return true;
		}
		else //if (ElevatorAvaible(p) && ElevatorFull)
		{
			HUMAN_AR[p].stop = false;
			return false;
		}
		//return false;

	}
	else if (WhenPeopleMeet(p, pos))
	{
		HUMAN_AR[p].stop = true;
		return true;
	}
	else if ((HUMAN_AR[p].pos_x + Human_width) == (Mainrect_x + Floor_width + Elevator_width - 10) && !ElevatorAvaible(p, pos))		//prawa sciana windy, dodac inne warunki 
	{
		HUMAN_AR[p].stop = true;
		return true;
	}
	//else if (ElevatorAvaible(p) /*&& ElevatorFull(p)*/)
	if (HUMAN_AR[p].stop == true) HUMAN_AR[p].stop = false;
	return false;
}

void animation(int time, int p, int pos)
{
	if (AnimationStop(p, pos)) return;

	HUMAN_AR[p].pos_x += 1;
	if (time % 10 < 5) HUMAN_AR[p].pos_y += 1;
	else HUMAN_AR[p].pos_y -= 1;
}

bool ElevatorLoaded(int pos)
{
	for (int i = 0; i < Hum_ID; i++)
	{
		if (HUMAN_AR[i].floor == abs(((pos - 10 - Mainrect_y) / Floor_height) - 4))
		{
			if (HUMAN_AR[i].pos_x >(Mainrect_x + Floor_width - 4))
			{
				if (i == Hum_ID - 1)	return true;
				continue;
			}
			else return false;
		}
	}
	return true;
}

void AnimationKill(int p)
{
	HUMAN buff[MAX_H];
	for (int i = 0; i < Hum_ID; i++)
	{
		if (i <= p)
		{
			if (i == p) continue;
			else buff[i] = HUMAN_AR[i];
		}
		else buff[i - 1] = HUMAN_AR[i];
	}
	Hum_ID--;
}

void AnimatationVertically(int p, int cur_y)
{
	HUMAN_AR[p].pos_y = cur_y + Floor_height - 11 - Human_height;
	HUMAN_AR[p].floor = abs(((cur_y - 10 - Mainrect_y) / Floor_height) - 4);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	


    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJEKTWINDA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJEKTWINDA));

    MSG msg;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

	GdiplusShutdown(gdiplusToken);

    return (int) msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJEKTWINDA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJEKTWINDA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   int button_size = 30;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, Mainrect_width + 200, Mainrect_height+30, nullptr, nullptr, hInstance, nullptr);

#pragma region Buttons

   HWND B_F4_0 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"0", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 0, 5 + margin + Floor_height * 0 + button_size * 2, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F4_0, hInstance, NULL);

   HWND B_F4_1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"1", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 1, 5 + margin + Floor_height * 0 + button_size * 1, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F4_1, hInstance, NULL);

   HWND B_F4_2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"2", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 0, 5 + margin + Floor_height * 0 + button_size * 1, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F4_2, hInstance, NULL);

   HWND B_F4_3 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"3", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 1, 5 + margin + Floor_height * 0 + button_size * 0, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F4_3, hInstance, NULL);

   HWND B_F4_4 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"-", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 0, 5 + margin + Floor_height * 0 + button_size * 0, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F4_4, hInstance, NULL);


   HWND B_F3_0 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"0", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 0, 5 + margin + Floor_height * 1 + button_size * 2, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F3_0, hInstance, NULL);

   HWND B_F3_1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"1", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 1, 5 + margin + Floor_height * 1 + button_size * 1, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F3_1, hInstance, NULL);

   HWND B_F3_2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"2", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 0, 5 + margin + Floor_height * 1 + button_size * 1, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F3_2, hInstance, NULL);

   HWND B_F3_3 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"-", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 1, 5 + margin + Floor_height * 1 + button_size * 0, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F3_3, hInstance, NULL);
																											  
   HWND B_F3_4 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"4", WS_CHILD | WS_VISIBLE | WS_BORDER,		  
	   40 + button_size * 0, 5 + margin + Floor_height * 1 + button_size * 0, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F3_4, hInstance, NULL);


   HWND B_F2_0 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"0", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 0, 5 + margin + Floor_height * 2 + button_size * 2, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F2_0, hInstance, NULL);

   HWND B_F2_1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"1", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 1, 5 + margin + Floor_height * 2 + button_size * 1, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F2_1, hInstance, NULL);

   HWND B_F2_2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"-", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 0, 5 + margin + Floor_height * 2 + button_size * 1, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F2_2, hInstance, NULL);

   HWND B_F2_3 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"3", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 1, 5 + margin + Floor_height * 2 + button_size * 0, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F2_3, hInstance, NULL);

   HWND B_F2_4 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"4", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 0, 5 + margin + Floor_height * 2 + button_size * 0, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F2_4, hInstance, NULL);


   HWND B_F1_0 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"0", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 0, 5 + margin + Floor_height * 3 + button_size * 2, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F1_0, hInstance, NULL);

   HWND B_F1_1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"-", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 1, 5 + margin + Floor_height * 3 + button_size * 1, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F1_1, hInstance, NULL);

   HWND B_F1_2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"2", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 0, 5 + margin + Floor_height * 3 + button_size * 1, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F1_2, hInstance, NULL);

   HWND B_F1_3 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"3", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 1, 5 + margin + Floor_height * 3 + button_size * 0, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F1_3, hInstance, NULL);

   HWND B_F1_4 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"4", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 0, 5 + margin + Floor_height * 3 + button_size * 0, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F1_4, hInstance, NULL);


   HWND B_F0_0 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"-", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 0, 5 + margin + Floor_height * 4 + button_size * 2, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F0_0, hInstance, NULL);

   HWND B_F0_1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"1", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 1, 5 + margin + Floor_height * 4 + button_size * 1, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F0_1, hInstance, NULL);

   HWND B_F0_2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"2", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 0, 5 + margin + Floor_height * 4 + button_size * 1, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F0_2, hInstance, NULL);

   HWND B_F0_3 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"3", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 1, 5 + margin + Floor_height * 4 + button_size * 0, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F0_3, hInstance, NULL);

   HWND B_F0_4 = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"4", WS_CHILD | WS_VISIBLE | WS_BORDER,
	   40 + button_size * 0, 5 + margin + Floor_height * 4 + button_size * 0, button_size, button_size, hWnd, (HMENU)ID_BUTTON_F0_4, hInstance, NULL);

#pragma endregion

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static bool first_update = true;

Pen Red_Pen(Color(255, 0, 0), 3);
Pen Black_Pen(Color(255, 0, 0, 0), 2);
Pen Green_Pen(Color(0, 255, 0), 3);
Pen Blue_Pen(Color(0, 0, 255), 2);

int up = -1;
int down = 1;

static int to;
static int where;
static int direction;

static ORDER buffor;
static int size = 0;
static int target;
static int elev_direction = 0;
static int main_target;
static bool stop;
static bool done = true;
static bool waited = false;

static int current_position = Mainrect_y + Floor_height * 4 + 10;

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
#pragma region Idbuttons
			case ID_BUTTON_F0_1:
				CreateHuman(0, 1);
				to = Mainrect_y + Floor_height * 4 + 10;
				where = Mainrect_y + Floor_height * 3 + 10;
				direction = up;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F0_2:
				CreateHuman(0, 2);
				to = Mainrect_y + Floor_height * 4 + 10;
				where = Mainrect_y + Floor_height * 2 + 10;
				direction = up;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F0_3:
				CreateHuman(0, 3);
				to = Mainrect_y + Floor_height * 4 + 10;
				where = Mainrect_y + Floor_height * 1 + 10;
				direction = up;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F0_4:
				CreateHuman(0, 4);
				to = Mainrect_y + Floor_height * 4 + 10;
				where = Mainrect_y + Floor_height * 0 + 10;
				direction = up;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F1_0:
				CreateHuman(1, 0);
				to = Mainrect_y + Floor_height * 3 + 10;
				where = Mainrect_y + Floor_height * 4 + 10;
				direction = down;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F1_2:
				CreateHuman(1, 2);
				to = Mainrect_y + Floor_height * 3 + 10;
				where = Mainrect_y + Floor_height * 2 + 10;
				direction = up;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F1_3:
				CreateHuman(1, 3);
				to = Mainrect_y + Floor_height * 3 + 10;
				where = Mainrect_y + Floor_height * 1 + 10;
				direction = up;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F1_4:
				CreateHuman(1, 4);
				to = Mainrect_y + Floor_height * 3 + 10;
				where = Mainrect_y + Floor_height * 0 + 10;
				direction = up;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F2_0:
				CreateHuman(2, 0);
				to = Mainrect_y + Floor_height * 2 + 10;
				where = Mainrect_y + Floor_height * 4 + 10;
				direction = down;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F2_1:
				CreateHuman(2, 1);
				to = Mainrect_y + Floor_height * 2 + 10;
				where = Mainrect_y + Floor_height * 3 + 10;
				direction = down;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F2_3:
				CreateHuman(2, 3);
				to = Mainrect_y + Floor_height * 2 + 10;
				where = Mainrect_y + Floor_height * 1 + 10;
				direction = up;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F2_4:
				CreateHuman(2, 4);
				to = Mainrect_y + Floor_height * 2 + 10;
				where = Mainrect_y + Floor_height * 0 + 10;
				direction = up;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F3_0:
				CreateHuman(3, 0);
				to = Mainrect_y + Floor_height * 1 + 10;
				where = Mainrect_y + Floor_height * 4 + 10;
				direction = down;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F3_1:
				CreateHuman(3, 1);
				to = Mainrect_y + Floor_height * 1 + 10;
				where = Mainrect_y + Floor_height * 3 + 10;
				direction = down;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F3_2:
				CreateHuman(3, 2);
				to = Mainrect_y + Floor_height * 1 + 10;
				where = Mainrect_y + Floor_height * 2 + 10;
				direction = down;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F3_4:
				CreateHuman(3, 4);
				to = Mainrect_y + Floor_height * 1 + 10;
				where = Mainrect_y + Floor_height * 0 + 10;
				direction = up;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F4_0:
				CreateHuman(4, 0);
				to = Mainrect_y + Floor_height * 0 + 10;
				where = Mainrect_y + Floor_height * 4 + 10;
				direction = down;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F4_1:
				CreateHuman(4, 1);
				to = Mainrect_y + Floor_height * 0 + 10;
				where = Mainrect_y + Floor_height * 3 + 10;
				direction = down;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F4_2:
				CreateHuman(4, 2);
				to = Mainrect_y + Floor_height * 0 + 10;
				where = Mainrect_y + Floor_height * 2 + 10;
				direction = down;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
			case ID_BUTTON_F4_3:
				CreateHuman(4, 3);
				to = Mainrect_y + Floor_height * 0 + 10;
				where = Mainrect_y + Floor_height * 1 + 10;
				direction = down;
				Load(to, where, direction, false, false);
				SetTimer(hWnd, TM_1, 25, NULL);
				SetTimer(hWnd, TM_3, 25, NULL);
				break;
#pragma endregion
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {		
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			Graphics graphics(hdc);
#pragma region Frame 
			
			// left wall of a shaft
			graphics.DrawLine(	&Black_Pen,
								Mainrect_x + Floor_width, 
								Mainrect_y + margin,
								Mainrect_x + Floor_width,
								Mainrect_y + Elevator_height);

			// right wall of a shaft
			graphics.DrawLine(	&Black_Pen,
								Mainrect_x + Floor_width + Elevator_width,
								Mainrect_y + margin, 
								Mainrect_x + Floor_width + Elevator_width, 
								Mainrect_y + Elevator_height);

			// floors
			for (int i = 1; i <= 5; i++) 
			{
				graphics.DrawLine(	&Black_Pen,
									Mainrect_x,
									Mainrect_y + Floor_height * i,
									Mainrect_x + Floor_width,
									Mainrect_y + Floor_height * i);
			}

			// exits
			for (int i = 1; i <= 5; i++)
			{
				graphics.DrawLine(	&Black_Pen,
									Mainrect_x + Floor_width + Elevator_width,
									Mainrect_y + Floor_height * i,
									Mainrect_x + Floor_width + Elevator_width + Exit_width,
									Mainrect_y + Floor_height * i);
			}

			// roof of a shaft
			graphics.DrawLine(	&Black_Pen,
								Mainrect_x + Floor_width,
								Mainrect_y + margin,
								Mainrect_x + Floor_width + Elevator_width,
								Mainrect_y + margin);

			// bottom of a shaft
			graphics.DrawLine(	&Black_Pen,
								Mainrect_x + Floor_width,
								Mainrect_y + Elevator_height,
								Mainrect_x + Floor_width + Elevator_width,
								Mainrect_y + Elevator_height);
#pragma endregion
			if (Hum_ID)
			{
				for (int j = 0; j < Hum_ID; j++)
				{
					graphics.DrawEllipse(&Blue_Pen, HUMAN_AR[j].pos_x + 1, (HUMAN_AR[j].pos_y), (Human_height / 5), (Human_height / 5));
					graphics.DrawLine(&Blue_Pen, (HUMAN_AR[j].pos_x) + (Human_width / 2), (HUMAN_AR[j].pos_y) + (Human_height / 5), HUMAN_AR[j].pos_x + (Human_width / 2), HUMAN_AR[j].pos_y + (7 * Human_height / 10));	//torso
					for (int i = 0; i < 2; i++) graphics.DrawLine(&Blue_Pen, HUMAN_AR[j].pos_x + (i * Human_width / 2), HUMAN_AR[j].pos_y + (3 * Human_height / 10) + i*(2 * Human_height / 10), HUMAN_AR[j].pos_x + (i + 1)*(Human_width / 2), HUMAN_AR[j].pos_y + (Human_height / 2) - i*(Human_height / 5));	//arms
					for (int i = 0; i < 2; i++) graphics.DrawLine(&Blue_Pen, HUMAN_AR[j].pos_x + (i * Human_width / 2), HUMAN_AR[j].pos_y + Human_height - i*(3 * Human_height / 10), HUMAN_AR[j].pos_x + (i + 1)*(Human_width / 2), HUMAN_AR[j].pos_y + (7 * Human_height / 10) + i*(3 * Human_height / 10));	//legs
				}

			}
			
			if (!Order.empty())
			{
				KillTimer(hWnd, TM_2);

				if (size < Order.size() && done || waited)
				{
					if (waited && current_position == main_target || Order.size() == 1)
					{
						size = Order.size();
						if (elev_direction != 0 && Order.size()!=1)
						{
							for (int i = 0; i < size; i++)
							{
								buffor = Order.at(i);
								buffor.b_to ? target = buffor.where : target = buffor.to;
								elev_direction == up
									?
								(target < main_target ? main_target = target : target = target)
									:
								(target > main_target ? main_target = target : target = target);
							}
						}

						if(elev_direction == 0 || elev_direction != 0 && Order.size() == 1)
						{
							buffor = Order.front();
							if (buffor.to != current_position)
							{
								if (current_position > buffor.to && current_position > buffor.where)
								{
									elev_direction = up;
									buffor.to < buffor.where ? main_target = buffor.to : main_target = buffor.where;
								}
								else if (current_position < buffor.to && current_position < buffor.where)
								{
									elev_direction = down;
									buffor.to > buffor.where ? main_target = buffor.to : main_target = buffor.where;
								}
								else if (current_position > buffor.to)
								{
									elev_direction = up;
									main_target = buffor.to;
								}
								else if (current_position < buffor.to)
								{
									elev_direction = down;
									main_target = buffor.to;
								}
							}
							else
							{
								main_target = buffor.where;
								buffor.b_to = true;
								Order.front() = buffor;
								if(elev_direction == 0) stop = true;
							}
							main_target < current_position ? elev_direction = up : elev_direction = down;
						}
					}
				}

				if (current_position != main_target && !stop)
				{
					waited = false;

						graphics.DrawRectangle(&Red_Pen,
							Mainrect_x + Floor_width + 10,
							current_position,
							Elevator_width - 4 * 5,
							Floor_height - 11);

						for (int i = 0; i < Hum_ID; i++)
						{
							if (HUMAN_AR[i].pos_x > (Mainrect_x + Floor_width - 10) && HUMAN_AR[i].pos_x < (Mainrect_x + Floor_width + Elevator_width - 10))
							{
								AnimatationVertically(i, current_position);
							}
						}

						current_position += 2 * elev_direction;

					if (current_position % Floor_height == Mainrect_y + 10 && current_position != main_target)
					{
						size = Order.size();
						for (int i = 0; i < size; i++)
						{
							buffor = Order.at(i);
							if (buffor.to == current_position && buffor.b_to == false && buffor.direction == elev_direction)
							{
								stop = true;
							}
							if (buffor.where == current_position && buffor.b_to == true)
							{
								stop = true;
							}
						}
					}
				}
				if (current_position == main_target && !stop && !waited)
				{
					elev_direction == up ? elev_direction = down : elev_direction = up;
					stop = true;
				}
				if (stop)
				{
					KillTimer(hWnd, TM_1);
					TIME_1 = 0;
					//SetTimer(hWnd, TM_2, 25, NULL);
					ElevatorOpen = true;

					graphics.DrawRectangle(&Green_Pen,
						Mainrect_x + Floor_width + 10,
						current_position,
						Elevator_width - 4 * 5,
						Floor_height - 11);

					if (ElevatorLoaded(current_position)) SetTimer(hWnd, TM_2, 25, NULL);
					
						//SetTimer(hWnd, TM_2, 25, NULL);
						if (TIME_2 == 0 || TIME_2 == 15)
						{
							size = Order.size();
							for (int i = 0; i < size; i++)
							{
								buffor = Order.at(i);
								if (current_position == buffor.to)
								{
									buffor.b_to = true;
									Order.at(i) = buffor;
								}
								if (current_position == buffor.where)
								{
									buffor.b_where = true;
									Order.at(i) = buffor;
								}
							}
						}


						if (TIME_2 > 15)
						{
							if (current_position == main_target)
							{
								for (int i = 0; i < size; i++)
								{
									buffor = Order.at(i);
									if (buffor.b_to && buffor.b_where)
									{
										Order.erase(Order.begin() + i);
										i--;
										size--;
									}
								}
								size = Order.size();
							}
							KillTimer(hWnd, TM_2);
							ElevatorOpen = false;
							TIME_2 = 0;
							waited = true;
							SetTimer(hWnd, TM_1, 25, NULL);
							stop = false;
						}
					
				
				}
			}
			else
			{
				KillTimer(hWnd, TM_1);
				SetTimer(hWnd, TM_2, 25, NULL);

				graphics.DrawRectangle(&Red_Pen,
					Mainrect_x + Floor_width + 10,
					current_position,
					Elevator_width - 4 * 5,
					Floor_height - 11);
				for (int i = 0; i < Hum_ID; i++)
				{
					if (HUMAN_AR[i].pos_x >(Mainrect_x + Floor_width - 10) && HUMAN_AR[i].pos_x < (Mainrect_x + Floor_width + Elevator_width - 10))
					{
						AnimatationVertically(i, current_position);
					}
				}

				elev_direction = 0;

				if (TIME_2 > 125 && current_position != Mainrect_y + Floor_height * 4 + 10)	//zmienic >
				{
					current_position += 2;
				}

				current_position == Mainrect_y + Floor_height * 4 + 10 ? TIME_2 = 0 : TIME_2 = TIME_2;
			}
			
            // TODO: Add any drawing code that uses hdc here...
			
			if (first_update)
			{
				InvalidateRect(hWnd, &MAINRECT, TRUE);
				first_update = false;
			}

            EndPaint(hWnd, &ps);
        }
        break;
	case WM_TIMER:
		switch(wParam)
			{
			case TM_1:
				InvalidateRect(hWnd, &ELEVATOR, TRUE);
				TIME_1++;
				break;
			case TM_2:
				InvalidateRect(hWnd, &ELEVATOR, TRUE);
				TIME_2++;
				break;
			case TM_3:
				if (Hum_ID != 0)
				{
					InvalidateRect(hWnd, &ELEVATOR, TRUE);
					InvalidateRect(hWnd, &FLOOR_0, TRUE);
					InvalidateRect(hWnd, &FLOOR_1, TRUE);
					InvalidateRect(hWnd, &FLOOR_2, TRUE);
					InvalidateRect(hWnd, &FLOOR_3, TRUE);
					InvalidateRect(hWnd, &FLOOR_4, TRUE);
					InvalidateRect(hWnd, &EXIT_0, TRUE);
					InvalidateRect(hWnd, &EXIT_1, TRUE);
					InvalidateRect(hWnd, &EXIT_2, TRUE);
					InvalidateRect(hWnd, &EXIT_3, TRUE);
					InvalidateRect(hWnd, &EXIT_4, TRUE);
					for (int p = 0; p < Hum_ID; p++)
					{
						//if (!HUMAN_AR[p].stop)
						//{
						if (HUMAN_AR[p].pos_x > Mainrect_x + Floor_width + Elevator_width + Exit_width)	AnimationKill(p);
						animation(++HUMAN_AR[p].time, p, current_position);
						//}
					}
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
