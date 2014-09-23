// мышь.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "мышь.h"
#include "Windowsx.h"

#define MAX_LOADSTRING 100

// Global Variables:

HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
POINT p1;
POINT p2;
HDC hdc;
HPEN hPen, hBlPen;
bool WhichPoint = true;
bool draw = false, pencil = false, line = false, ellipse = false, rectangle = false, eraser = false, lastAction=false;

void MyDraw(HDC hdc);
// Forward declarations of functions included in this code module:
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

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));
	

	
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+10);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   HMENU MainMenu = CreateMenu();
   HMENU hPopupMenu = CreatePopupMenu();

   AppendMenu(MainMenu, MF_STRING | MF_POPUP, 1, L"&Карандаш");

   AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu, L"&Фигуры");
   {	  

	   AppendMenu(hPopupMenu, MF_STRING, 2, L"Линия");
	   AppendMenu(hPopupMenu, MF_STRING, 3, L"Овал");
	   AppendMenu(hPopupMenu, MF_STRING, 4, L"Прямоугольник");
   }

   AppendMenu(MainMenu, MF_STRING | MF_POPUP, 5, L"&Ластик");
   AppendMenu(MainMenu, MF_STRING | MF_POPUP, 6, L"&Отменить стирание");
   



   SetMenu(hWnd, MainMenu);
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
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch (message)
	{
	case WM_COMMAND:
		switch (wParam)
		{
		case 1:
			pencil = true; line = false; ellipse = false; rectangle = false; eraser = false; lastAction = false;
			hBlPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			return 0;
		case 2:
			line = true; pencil =  false; ellipse = false; rectangle = false; eraser = false; lastAction = false;
			return 0;
		case 3:
			ellipse = true; pencil = false; line = false;  rectangle = false; eraser = false; lastAction = false;
			return 0;
		case 4:
			rectangle = true; pencil = false; line = false; ellipse = false;  eraser = false; lastAction = false;
			return 0;
		case 5:
			eraser = true;
			hPen = CreatePen(PS_SOLID, 20, RGB(255, 255, 255));
			
			return 0;
		case 6:
			lastAction = true;
			InvalidateRect(hWnd, NULL, false);
			return 0;
		}
		
		break;
	case WM_LBUTTONDOWN:
		
		hdc = GetDC(hWnd);
		
		if (lastAction){
			MyDraw(hdc);
		}

		if ((pencil)|(eraser)){

			p1.x = GET_X_LPARAM(lParam);
			p1.y = GET_Y_LPARAM(lParam);
			draw = true;
		}
		else if ((line) | (ellipse) | (rectangle)){
			if (WhichPoint){
				p1.x = LOWORD(lParam);
				p1.y = HIWORD(lParam);
				WhichPoint = !WhichPoint;
			}
			else{
				p2.x = LOWORD(lParam);
				p2.y = HIWORD(lParam);
				WhichPoint = !WhichPoint;
				InvalidateRect(hWnd, NULL, false);
			}
		}
		return 0;

	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		if ((draw == true)&(pencil == true)){ SelectObject(hdc, hBlPen);  }
		if ((draw == true)&(eraser == true)){SelectObject(hdc, hPen); }
		if (((draw == true)&(pencil == true)) | ((draw == true)&(eraser == true)))
		{
			
			MoveToEx( hdc,p1.x,p1.y,NULL);
			p1.x = GET_X_LPARAM(lParam);
			p1.y = GET_Y_LPARAM(lParam);
			LineTo(hdc,p1.x,p1.y);
			ReleaseDC(hWnd, hdc);
		}
		
			return 0;
		
	case WM_LBUTTONUP:

		draw = false;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);		
		// TODO: Add any drawing code here...
		MoveToEx(hdc, p1.x, p1.y, NULL);
		MyDraw(hdc);
		EndPaint(hWnd, &ps);
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

void MyDraw(HDC hdc)
{
	if (ellipse){
		Ellipse(hdc, p1.x, p1.y, p2.x, p2.y);
	}
	if (rectangle){
		Rectangle(hdc, p1.x, p1.y, p2.x, p2.y);
	}
	if (line){
		LineTo(hdc, p2.x, p2.y);
	}
	if (lastAction){
		
		
	}
	
}