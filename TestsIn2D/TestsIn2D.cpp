// TestsIn2D.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TestsIn2D.h"

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <ctime>
#include <iostream>
#include <assert.h>
#include "ParabolaTarget.h"
#include "ParabolaOrigin.h"
#include "ParabolaProjectile.h"

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

ParabolaTarget* parabolaTarget = nullptr;
ParabolaOrigin* parabolaOrigin = nullptr;
ParabolaProjectile* parabolaProjectile = nullptr;

VOID OnPaint(HDC hdc, Gdiplus::Graphics& graphics)
{
	if (parabolaTarget && parabolaOrigin && parabolaProjectile)
	{
		parabolaTarget->Draw(hdc, graphics);
		parabolaOrigin->Draw(hdc, graphics);

		parabolaProjectile->Move();
		parabolaProjectile->Draw(hdc, graphics);
	}
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
   HWND                hWnd;
   MSG                 msg;
   WNDCLASS            wndClass;
   GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR           gdiplusToken;
   
   // Initialize GDI+.
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   
   wndClass.style          = CS_HREDRAW | CS_VREDRAW;
   wndClass.lpfnWndProc    = WndProc;
   wndClass.cbClsExtra     = 0;
   wndClass.cbWndExtra     = 0;
   wndClass.hInstance      = hInstance;
   wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
   wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
   wndClass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
   wndClass.lpszMenuName   = NULL;
   wndClass.lpszClassName  = TEXT("2DTests");
   
   RegisterClass(&wndClass);
   
   hWnd = CreateWindow(
      TEXT("2DTests"),   // window class name
      TEXT("2D Tests"),  // window caption
      WS_OVERLAPPEDWINDOW,      // window style
      0,            // initial x position
      0,            // initial y position
      640,            // initial x size
      480,            // initial y size
      NULL,                     // parent window handle
      NULL,                     // window menu handle
      hInstance,                // program instance handle
      NULL);                    // creation parameters
	  
   ShowWindow(hWnd, iCmdShow);
   UpdateWindow(hWnd);

   parabolaTarget = new ParabolaTarget();
   parabolaOrigin = new ParabolaOrigin();
   parabolaProjectile = new ParabolaProjectile(parabolaOrigin, parabolaTarget);
   
	bool done = false;
	while(!done) // loop that runs while done = FALSE
	{	
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) // is there a message waiting?
		{
			if (msg.message==WM_QUIT){ // is this quit message?
				done=TRUE;}	// if so done = TRUE
			else // not a quit message (so send message to message handler WndProc)
			{
				TranslateMessage(&msg);	// translate the message
				DispatchMessage(&msg); // dispatch the message
			}
		}
		else // their are no messages
		{
			InvalidateRect(hWnd, NULL, TRUE);//force window to repaint
			UpdateWindow(hWnd);
		} // end if peekMessage
	} // end while not done 

	delete parabolaProjectile;
	delete parabolaTarget;
	delete parabolaOrigin;

   GdiplusShutdown(gdiplusToken);
   return (int)msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;
	PAINTSTRUCT  ps;

	switch(message)
	{
	case WM_PAINT:
	{
		assert(hWnd);
		hdc = BeginPaint(hWnd, &ps);
		assert(hdc);

		// -------------- for double buffering purposes
		RECT rcClient;
		::GetClientRect(hWnd, &rcClient);
		int left = rcClient.left;
		int top = rcClient.top;
		int width = rcClient.right - rcClient.left;
		int height = rcClient.bottom - rcClient.top;

		static HDC hdcMem = ::CreateCompatibleDC(hdc); // NOTE: when this was not static, program crashed quickly
		assert(hdcMem);
		const int nMemDC = ::SaveDC(hdcMem);

		static HBITMAP hBitmap = ::CreateCompatibleBitmap(hdc, width, height); // NOTE: when this was not static, program crashed quickly
		assert(hBitmap);
		::SelectObject(hdcMem, hBitmap);

		Graphics graphics(hdcMem);
		SolidBrush back(Color(0,0,0));
		graphics.FillRectangle(&back, left, top, width, height);
		// -------------------------------------------

		OnPaint(hdcMem, graphics);
      
		//---------------------------- for double buffering
		RECT rcClip;
	    ::GetClipBox(hdc, &rcClip);
	    left = rcClip.left;
	    top = rcClip.top;
	    width = rcClip.right - rcClip.left;
	    height = rcClip.bottom - rcClip.top;
	    ::BitBlt(hdc, left, top, width, height, hdcMem, left, top, SRCCOPY);

	    ::RestoreDC(hdcMem, nMemDC);
	    //::DeleteObject(hBitmap); // NOTE: this was not commented out when hBitmap was not static
		//--------------------------------------------
		
		EndPaint(hWnd, &ps);
		return 0;
	}	
	case WM_ERASEBKGND:
			return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN: // a key is being held down
		switch (wParam)
		{
			case VK_ESCAPE: // escape key
				PostQuitMessage(WM_QUIT); // post quit message 
				break;
			case VK_RETURN:
				break;
			case VK_SPACE:
				parabolaProjectile->Go();
				break;
			case VK_LEFT:
				parabolaTarget->MoveLeft();
				break;
			case VK_RIGHT:
				parabolaTarget->MoveRight();
				break;
			case VK_UP:
				parabolaTarget->MoveUp();
				break;
			case VK_DOWN:
				parabolaTarget->MoveDown();
				break;
			case VK_F1:
				parabolaProjectile->flightDuration += 1.0f;
				break;
			case VK_F2:
				parabolaProjectile->flightDuration -= 1.0f;
				break;
			case VK_F3:
				parabolaProjectile->arc += 0.05f;
				break;
			case VK_F4:
				parabolaProjectile->arc -= 0.05f;
				break;
			default: break;
		}							
		return 0;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
   }
} // WndProc