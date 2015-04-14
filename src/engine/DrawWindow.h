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

#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H

#ifdef _X11
#include <X11/Xlib.h>
enum DrawWindow_Keycodes
{
	KEY_BACKSPACE = 22,
	KEY_RETURN = 36,
	KEY_LEFT = 113,
	KEY_RIGHT = 114,
	KEY_DOWN = 116,
	KEY_UP = 111
};
#elif _WIN32
#include <windows.h>
enum DrawWindow_Keycodes
{
	KEY_BACKSPACE = VK_BACK,
	KEY_RETURN = VK_RETURN,
	KEY_LEFT = VK_LEFT,
	KEY_RIGHT = VK_RIGHT,
	KEY_DOWN = VK_DOWN,
	KEY_UP = VK_UP,
	KEY_SPACE = VK_SPACE
};
#endif

#define MOUSE_DOWN 0
#define MOUSE_UP 1
#define MOUSE_MOVE 2
#define EXIT 3

class Bitmap;

struct DrawWindowInternal
{
#ifdef _X11
	unsigned long bg, fg;
	Display * dsp;
	int screenNumber;
	Window win;
	XEvent evt;
	GC gc;
	long eventMask;
	Atom wmDeleteMessage;
	Visual *visual;
	Font font;
	Pixmap backBuffer;
#elif _WIN32
	WNDCLASSEX wc;
	HWND hwnd;
	LPCTSTR name;
	char classname[50];
	COLORREF fg_color;
	COLORREF bg_color;
	HBITMAP hbmp;
	MSG msg;
	bool msgReady;
	HFONT font;
	HDC hdc;
#endif
};

class DrawWindow
{
public:
	//initialize these variables, before Open
	int x, y, width, height;

	//override event functions:
	void (*mouseDownFunction)(int x, int y);
	void (*mouseUpFunction)(int x, int y);
	void (*mouseMoveFunction)(int x, int y);
	void (*keyDownFunction)(char keyCharOrCode, bool isKeyCode);
	void (*keyUpFunction)(char keyCharOrCode, bool isKeyCode);
	void (*exitFunction)();

	void open();
	void close();

	void setBackgroundColor(int r, int g, int b);
	void setBackgroundBitmap(Bitmap * bmp);
	void setForegroundColor(int r, int g, int b);

	void drawLine(int x1, int y1, int x2, int y2);
	void drawString(int x, int y, char * str);
	void drawBitmap(Bitmap * bitmap, int x, int y);

	void clear();
	void present();

	void doNextEvent();
	int numNextEvent();

	//returns time in microseconds (1000 milliseconds)
	unsigned long getTime();
	void sleep(unsigned long time);

	DrawWindow();
	~DrawWindow();

	//dont touch these:
	struct DrawWindowInternal * dwi;
	void onResize(int newWidth, int newHeight);

private:
	bool opened;
	Bitmap * bgBitmap;

};

#endif
