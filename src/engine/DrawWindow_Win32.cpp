/*
 * Copyright (c) 2015 Evan Kale
 * Email: EvanKale91@gmail.com
 * Website: www.ISeeDeadPixel.com
 *          www.evankale.blogspot.ca
 *
 * This file is part of Breakout.
 *
 * Breakout is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef _WIN32

#include "engine/DrawWindow.h"
#include "engine/Bitmap.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

DrawWindow * current_window;

LRESULT CALLBACK DrawWindow_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_CLOSE)
	{
		DestroyWindow(hwnd);
		delete(current_window);
	}
	else if(msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		exit(0);
	}
	else if(msg == WM_PAINT)
	{
		if(!current_window)
		{
		}
		else if(!current_window->dwi->hbmp)
		{
			current_window->dwi->hbmp = CreateBitmap(current_window->width,current_window->height,1,32,NULL);

			if(current_window->dwi->hbmp == NULL)
				MessageBox(hwnd, "Could not create bitmap", "Error", MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			PAINTSTRUCT ps;
			HDC hdc, hdcMem;

			hdc = BeginPaint(current_window->dwi->hwnd, &ps);

			hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, current_window->dwi->hbmp);

			BitBlt(hdc, 0, 0, current_window->width, current_window->height, hdcMem, 0, 0, SRCCOPY);

			DeleteDC(hdcMem);

			EndPaint(current_window->dwi->hwnd, &ps);
		}
	}
	else if(msg == WM_LBUTTONDOWN && current_window->mouseDownFunction)
	{
		int mx = LOWORD(lParam);
		int my = HIWORD(lParam);

		if(!(mx==0 && my ==0))
		{
			current_window->mouseDownFunction(mx, my);
		}
	}
	else if(msg == WM_LBUTTONUP && current_window->mouseUpFunction)
	{
		int mx = LOWORD(lParam);
		int my = HIWORD(lParam);

		current_window->mouseUpFunction(mx, my);
	}
	else if(msg == WM_MOUSEMOVE && current_window->mouseMoveFunction)
	{
		int mx = LOWORD(lParam);
		int my = HIWORD(lParam);

		current_window->mouseMoveFunction(mx, my);
	}
	else if(msg == WM_KEYDOWN && current_window->keyDownFunction)
	{
		int keycode = wParam;

		if(0x30 <= keycode && keycode <= 0x5A)
			current_window->keyDownFunction(keycode, 0);
		else
			current_window->keyDownFunction(keycode, 1);
	}
	else if(msg == WM_KEYUP && current_window->keyUpFunction)
	{
		int keycode = wParam;

		if(0x30 <= keycode && keycode <= 0x5A)
			current_window->keyUpFunction(keycode, 0);
		else
			current_window->keyUpFunction(keycode, 1);
	}
	else if(msg == WM_SIZE)
	{
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);
		current_window->onResize(width,height);
	}
	else if(msg == WM_ERASEBKGND)
		return 1;

	else
		return DefWindowProc(hwnd,msg,wParam,lParam);
}

DrawWindow::DrawWindow()
{
	memset(this,0,sizeof(DrawWindow));
}

DrawWindow::~DrawWindow()
{
	if(opened)
	close();
}

void DrawWindow::open()
{
	dwi = (DrawWindowInternal*)calloc(sizeof(struct DrawWindowInternal),1);

	strcpy(dwi->classname,"DrawWindow_");

	dwi->wc.cbSize = sizeof(WNDCLASSEX);
	dwi->wc.style = 0;
	dwi->wc.lpfnWndProc = DrawWindow_WndProc;
	dwi->wc.cbClsExtra = 0;
	dwi->wc.cbWndExtra = 0;
	dwi->wc.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	dwi->wc.hIcon = LoadIcon (NULL,IDI_APPLICATION);
	dwi->wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	dwi->wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	dwi->wc.lpszMenuName = NULL;
	dwi->wc.lpszClassName = dwi->classname;
	dwi->wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

	if(!RegisterClassEx(&(dwi->wc)))
	{
		MessageBox(NULL, "Failed to register window class",
			"Error", MB_ICONEXCLAMATION | MB_OK );
	}

	dwi->hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		dwi->classname,
		"WIN32",
		WS_OVERLAPPEDWINDOW,
		x, y, width, height,
		NULL, NULL, dwi->wc.hInstance, NULL);

	if(dwi->hwnd == NULL)
	{
		MessageBox(NULL, "Failed to create window","Error", MB_ICONEXCLAMATION | MB_OK );
		return;
	}

	ShowWindow(dwi->hwnd,1);
	UpdateWindow(dwi->hwnd);

	current_window = this;

	dwi->font = CreateFont(18,7,0,0,0,0,0,0,0,0,0,0,0,"Arial");
	dwi->hdc = GetDC(dwi->hwnd);

	opened = true;
}

void DrawWindow::clear ()
{
	if(bgBitmap)
	{
		int numX = width/bgBitmap->width +1;
		int numY = height/bgBitmap->height +1;

		for(int x = 0; x<numX; ++x)
		for(int y=0; y<numY; ++y)
		drawBitmap(bgBitmap,x*bgBitmap->width,y*bgBitmap->height);
	}
	else
	{
		HDC tmpDC;

		HBRUSH tmpBrush;
		RECT rect;

		tmpDC = CreateCompatibleDC(dwi->hdc);

		tmpBrush = CreateSolidBrush(dwi->bg_color);
		SelectObject(tmpDC,dwi->hbmp);

		rect.left = 0;
		rect.top = 0;
		rect.right = width;
		rect.bottom = height;

		FillRect(tmpDC, &rect, tmpBrush);

		DeleteObject(tmpBrush);
		DeleteDC(tmpDC);
	}
}

void DrawWindow::setForegroundColor(int r, int g, int b)
{
	dwi->fg_color = RGB(r,g,b);
}

void DrawWindow::setBackgroundColor(int r, int g, int b)
{
	dwi->bg_color = RGB(r,g,b);
}

void DrawWindow::setBackgroundBitmap(Bitmap * bmp)
{
	bgBitmap = bmp;
}

void DrawWindow::drawLine (int x1, int y1, int x2, int y2)
{
	POINT pnts[2];
	HDC tmpDC;
	HPEN tmpPen;
	HGDIOBJ oldPen;

	POINT p1 =
	{	x1,y1};
	POINT p2 =
	{	x2,y2};

	pnts[0] = p1;
	pnts[1] = p2;

	tmpDC = CreateCompatibleDC(dwi->hdc);

	tmpPen = CreatePen(0, 1, dwi->fg_color);

	SelectObject(tmpDC,dwi->hbmp);
	oldPen = SelectObject(tmpDC,tmpPen);

	Polyline(tmpDC, pnts, 2);

	SelectObject(tmpDC,oldPen);

	DeleteObject(tmpPen);
	DeleteDC(tmpDC);
}

void DrawWindow::drawString (int x, int y, char * str)
{
	HDC tmpDC;
	HFONT tmpFont;
	HGDIOBJ oldFont;

	tmpDC = CreateCompatibleDC(dwi->hdc);

	SelectObject(tmpDC,dwi->hbmp);
	oldFont = SelectObject(tmpDC,dwi->font);

	//Polyline(tmpDC, pnts, 2);
	SetBkMode(tmpDC, TRANSPARENT);
	SetTextColor(tmpDC, dwi->fg_color);
	TextOut(tmpDC,x,y,str,strlen(str));

	SelectObject(tmpDC,oldFont);

	DeleteDC(tmpDC);
}

void DrawWindow::drawBitmap (Bitmap * bitmap, int x, int y)
{
	HDC tmpDC;

	tmpDC = CreateCompatibleDC(dwi->hdc);
	SelectObject(tmpDC,dwi->hbmp);

	BitBlt(tmpDC, x, y, bitmap->width, bitmap->height, bitmap->hdcBmp_mask, 0, 0, SRCAND);
	BitBlt(tmpDC, x, y, bitmap->width, bitmap->height, bitmap->hdcBmp, 0, 0, SRCPAINT);

	DeleteDC(tmpDC);
}

void DrawWindow::onResize(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;

	//free current backbuffer
	if(current_window->dwi->hbmp)
	{
		DeleteObject(current_window->dwi->hbmp);
		current_window->dwi->hbmp = 0;
	}

	//WM_PAINT event will automatically recreate backbuffer
}

void DrawWindow::doNextEvent()
{
	if(dwi->msgReady)
	{
		TranslateMessage(&dwi->msg);

		if(dwi->msg.message == WM_CLOSE)
		{
			DispatchMessage(&dwi->msg);
		}
		else
		{
			DispatchMessage(&dwi->msg);
			dwi->msgReady = 0;
		}
	}
}

int DrawWindow::numNextEvent()
{
	if(!dwi->msgReady)
	{
		dwi->msgReady = PeekMessage(&dwi->msg, dwi->hwnd, 0, 0, PM_REMOVE);
	}
	return dwi->msgReady;
}

void DrawWindow::close ()
{
	if(dwi->hbmp)
	{
		DeleteObject(dwi->hbmp);
		dwi->hbmp = 0;
	}
	if(dwi->font)
	{
		DeleteObject(dwi->font);
		dwi->font = 0;
	}
	if(dwi->hdc)
	{
		DeleteDC(dwi->hdc);
		dwi->hdc = 0;
	}

	free(dwi);
	dwi = 0;

	opened = false;
}

void DrawWindow::present()
{
	struct tagRECT rcClient;
	GetClientRect(dwi->hwnd, &rcClient);
	InvalidateRect(dwi->hwnd, &rcClient, 1);
}

unsigned long DrawWindow::getTime()
{
	//TODO: replace this method with something more precise (GetTickCount only has 15ms accuracy)
	return GetTickCount();
}

void DrawWindow::sleep(unsigned long time)
{
	Sleep(time);
}

#endif
