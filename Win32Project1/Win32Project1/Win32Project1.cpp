// Win32Project1.cpp : Defines the entry point for the application.

#include "stdafx.h"
#include "Win32Project1.h"
#include <Windows.h>
#include "teddy.h"
#include "FBX.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

using namespace DirectX;



#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hWnd;
//myDirectX _myDirectX;

int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{



	teddy t(hInstance);

	if (!t.init()) return 1;

	return t.run();
}




//// Forward declarations of functions included in this code module:
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//                     _In_opt_ HINSTANCE hPrevInstance,
//                     _In_ LPWSTR    lpCmdLine,
//                     _In_ int       nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//    // TODO: Place code here.
//
//    // Initialize global strings
//    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
//    MyRegisterClass(hInstance);
//
//    // Perform application initialization:
//    if (!InitInstance (hInstance, nCmdShow))
//    {
//        return FALSE;
//    }
//
//	_myDirectX.init(hWnd, 500, 500);
//
////#ifndef NDEBUG
////	AllocConsole();
////	FILE* new_std_in_out;
////	freopen_s(&new_std_in_out, "CONOUT$", "w", stdout);
////	freopen_s(&new_std_in_out, "CONIN$", "r", stdin);
////	std::cout << "Hello world!\n";
////#endif
//
//    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));
//
//	MSG msg = { 0 };
//
//    // Main message loop:
//    while (WM_QUIT != msg.message)
//    {
//		PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
//
//        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//
//		_myDirectX.render();
//		
//    }
//
//#ifndef NDEBUG
//	FreeConsole();
//#endif
//
//    return (int) msg.wParam;
//}



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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
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

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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

//void SetNewCursorClick()
//{
//	GetCursorPos(&CurrMousePos);
//	ScreenToClient(hWnd, &CurrMousePos);
//	PrevMousePos = CurrMousePos;
//}
//
//
//
//void SetMousePos(void)
//{
//	PrevMousePos = CurrMousePos;
//	GetCursorPos(&CurrMousePos);
//	ScreenToClient(hWnd, &CurrMousePos);
//}
//
//
//void UpdateCamera(float dt)
//{
//	if (GetAsyncKeyState('W'))
//	{
//		XMMATRIX Translation = XMMatrixTranslation(0.0f, 0.0f, Speed);
//		XMMATRIX Temp = XMLoadFloat4x4(&dCamera);
//		XMMATRIX Result = XMMatrixMultiply(Translation, Temp);
//		XMStoreFloat4x4(&dCamera, Result);
//	}
//	else if (GetAsyncKeyState('S'))
//	{
//		XMMATRIX Translation = XMMatrixTranslation(0.0f, 0.0f, -Speed);
//		XMMATRIX Temp = XMLoadFloat4x4(&dCamera);
//		XMMATRIX Result = XMMatrixMultiply(Translation, Temp);
//		XMStoreFloat4x4(&dCamera, Result);
//	}
//
//	if (GetAsyncKeyState('A'))
//	{
//		XMMATRIX Translation = XMMatrixTranslation(-Speed, 0.0f, 0.0f);
//		XMMATRIX Temp = XMLoadFloat4x4(&dCamera);
//		XMMATRIX Result = XMMatrixMultiply(Translation, Temp);
//		XMStoreFloat4x4(&dCamera, Result);
//	}
//	else if (GetAsyncKeyState('D'))
//	{
//		XMMATRIX Translation = XMMatrixTranslation(Speed, 0.0f, 0.0f);
//		XMMATRIX Temp = XMLoadFloat4x4(&dCamera);
//		XMMATRIX Result = XMMatrixMultiply(Translation, Temp);
//		XMStoreFloat4x4(&dCamera, Result);
//	}
//
//	/*if (GetAsyncKeyState(VK_NUMPAD0))
//	{
//	if (DEBUG_RENDER == 1)
//	DEBUG_RENDER = 0;
//	else
//	DEBUG_RENDER = 1;
//	}*/
//
//	if (GetAsyncKeyState(VK_UP))
//	{
//		XMMATRIX Translation = XMMatrixTranslation(0.0f, Speed, 0.0f);
//		XMMATRIX Temp = XMLoadFloat4x4(&dCamera);
//		XMMATRIX Result = XMMatrixMultiply(Translation, Temp);
//		XMStoreFloat4x4(&dCamera, Result);
//	}
//	else if (GetAsyncKeyState(VK_DOWN))
//	{
//		XMMATRIX Translation = XMMatrixTranslation(0.0f, -Speed, 0.0f);
//		XMMATRIX Temp = XMLoadFloat4x4(&dCamera);
//		XMMATRIX Result = XMMatrixMultiply(Translation, Temp);
//		XMStoreFloat4x4(&dCamera, Result);
//	}
//
//	if (RMouseClick)
//	{
//		float dx = CurrMousePos.x - PrevMousePos.x;
//		float dy = CurrMousePos.y - PrevMousePos.y;
//
//		XMFLOAT4 Pos = XMFLOAT4(dCamera._41, dCamera._42, dCamera._43, dCamera._44);
//
//		dCamera._41 = 0.0f;
//		dCamera._42 = 0.0f;
//		dCamera._43 = 0.0f;
//
//		XMMATRIX RotationX = XMMatrixRotationX(dy * 0.01f);
//		XMMATRIX RotationY = XMMatrixRotationY(dx * 0.01f);
//
//		XMMATRIX Temp = XMLoadFloat4x4(&dCamera);
//		Temp = XMMatrixMultiply(RotationX, Temp);
//		Temp = XMMatrixMultiply(Temp, RotationY);
//
//		XMStoreFloat4x4(&dCamera, Temp);
//
//		dCamera._41 = Pos.x;
//		dCamera._42 = Pos.y;
//		dCamera._43 = Pos.z;
//
//		SetMousePos();
//	}
//}
//



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


	/*if (message == WM_RBUTTONDOWN)
	{
		RMouseClick = true;
		SetNewCursorClick();
	}
	else if (message == WM_RBUTTONUP)
		RMouseClick = false;*/
    switch (message)
    {
    //case WM_COMMAND:
    //    {
    //        int wmId = LOWORD(wParam);
    //        // Parse the menu selections:
    //        switch (wmId)
    //        {
    //        case IDM_ABOUT:
    //            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
    //            break;
    //        case IDM_EXIT:
    //            DestroyWindow(hWnd);
    //            break;
    //        default:
    //            return DefWindowProc(hWnd, message, wParam, lParam);
    //        }
    //    }
    //    break;
    //case WM_PAINT:
    //    {
    //        PAINTSTRUCT ps;
    //        HDC hdc = BeginPaint(hWnd, &ps);
    //        // TODO: Add any drawing code that uses hdc here...
    //        EndPaint(hWnd, &ps);
    //    }
    //    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    UNREFERENCED_PARAMETER(lParam);
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        return (INT_PTR)TRUE;
//
//    case WM_COMMAND:
//        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//        {
//            EndDialog(hDlg, LOWORD(wParam));
//            return (INT_PTR)TRUE;
//        }
//        break;
//    }
//    return (INT_PTR)FALSE;
//}
