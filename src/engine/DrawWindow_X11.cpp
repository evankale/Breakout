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

#ifdef _X11

#include "engine/DrawWindow.h"
#include "engine/Bitmap.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xutil.h>
#include <sys/time.h>
#include <unistd.h>

DrawWindow::DrawWindow()
{
	memset(this, 0, sizeof(DrawWindow));
}

DrawWindow::~DrawWindow()
{
	if (opened)
		close();
}

void DrawWindow::open()
{
	dwi = (DrawWindowInternal*) malloc(sizeof(struct DrawWindowInternal));

	dwi->dsp = XOpenDisplay(NULL);

	dwi->screenNumber = DefaultScreen(dwi->dsp);

	dwi->win = XCreateSimpleWindow(dwi->dsp, DefaultRootWindow(dwi->dsp), x, y, width, height, 0, 0, 0);

	XMapWindow(dwi->dsp, dwi->win);

	dwi->eventMask = StructureNotifyMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask;
	XSelectInput(dwi->dsp, dwi->win, dwi->eventMask);

	do
	{
		XNextEvent(dwi->dsp, &(dwi->evt));
	}
	while ((dwi->evt).type != MapNotify);

	dwi->gc = XCreateGC(dwi->dsp, dwi->win, 0, NULL);

	dwi->wmDeleteMessage = XInternAtom(dwi->dsp, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(dwi->dsp, dwi->win, &(dwi->wmDeleteMessage), 1);

	dwi->visual = DefaultVisual(dwi->dsp, dwi->screenNumber);

	dwi->backBuffer = XCreatePixmap(dwi->dsp, dwi->win, width, height, 24);

	//dwi->font = XLoadFont(dwi->dsp, "-adobe-helvetica-medium-r-normal--20-*-*-*-p-*-iso8859-1");
	//XSetFont(dwi->dsp,dwi->gc,dwi->font);

	opened = true;
}

void DrawWindow::clear()
{
	XSetForeground(dwi->dsp, dwi->gc, dwi->bg);
	if (bgBitmap)
	{
		int numX = width / bgBitmap->width + 1;
		int numY = height / bgBitmap->height + 1;

		for (int x = 0; x < numX; ++x)
			for (int y = 0; y < numY; ++y)
				drawBitmap(bgBitmap, x * bgBitmap->width, y * bgBitmap->height);
	}
	else
		XFillRectangle(dwi->dsp, dwi->backBuffer, dwi->gc, 0, 0, width, height);
}

void DrawWindow::setForegroundColor(int r, int g, int b)
{
	dwi->fg = r << 16 | g << 8 | b << 0;
}

void DrawWindow::setBackgroundColor(int r, int g, int b)
{
	dwi->bg = r << 16 | g << 8 | b << 0;
}

void DrawWindow::setBackgroundBitmap(Bitmap * bmp)
{
	bgBitmap = bmp;
}

void DrawWindow::drawLine(int x1, int y1, int x2, int y2)
{
	XSetForeground(dwi->dsp, dwi->gc, dwi->fg);
	XDrawLine(dwi->dsp, dwi->backBuffer, dwi->gc, x1, y1, x2, y2);
}

void DrawWindow::drawString(int x, int y, char * str)
{
	XSetForeground(dwi->dsp, dwi->gc, dwi->fg);
	XDrawString(dwi->dsp, dwi->backBuffer, dwi->gc, x, y, str, strlen(str));
}

void DrawWindow::drawBitmap(Bitmap * bitmap, int x, int y)
{
	if (bitmap)
	{
		XSetClipMask(dwi->dsp, dwi->gc, bitmap->pixmap);
		XSetClipOrigin(dwi->dsp, dwi->gc, x, y);
		XPutImage(dwi->dsp, dwi->backBuffer, dwi->gc, bitmap->ximage, 0, 0, x, y, bitmap->width, bitmap->height);
		XSetClipMask(dwi->dsp, dwi->gc, 0);
	}
}

void DrawWindow::doNextEvent()
{
	XNextEvent(dwi->dsp, &(dwi->evt));

	if ((dwi->evt).type == ButtonRelease && mouseUpFunction)
	{
		mouseUpFunction((dwi->evt).xbutton.x, (dwi->evt).xbutton.y);
	}
	else if ((dwi->evt).type == ButtonPress && mouseDownFunction)
	{
		mouseDownFunction((dwi->evt).xbutton.x, (dwi->evt).xbutton.y);
	}
	else if ((dwi->evt).type == MotionNotify && mouseMoveFunction)
	{
		mouseMoveFunction((dwi->evt).xbutton.x, (dwi->evt).xbutton.y);
	}
	else if ((dwi->evt).type == KeyPress && keyDownFunction)
	{
		KeySym key;
		char text[10];

		int result = XLookupString((XKeyEvent *) &(dwi->evt), 	// the keyboard event
		    text, 					// buffer when text will be written
		    10, 			        // size of the text buffer
		    &key, 					// workstation-independent key symbol
		    NULL);					// pointer to a composeStatus structure (unused)
		if (result == 1)
		{
			char keyChar = text[0];

			if (('0' <= keyChar && keyChar <= '9') || ('A' <= keyChar && keyChar <= 'Z'))
			{
				keyDownFunction(keyChar, 0);
			}
			else if ('a' <= keyChar && keyChar <= 'z')
			{
				//capitalize it
				keyDownFunction(keyChar - 32, 0);
			}
			else
			{
				keyDownFunction(((XKeyEvent *) &(dwi->evt))->keycode, 1);
			}
		}
		else
			keyDownFunction(((XKeyEvent *) &(dwi->evt))->keycode, 1);

	}
	else if ((dwi->evt).type == KeyRelease && keyUpFunction)
	{
		KeySym key;
		char text[10];

		int result = XLookupString((XKeyEvent *) &(dwi->evt), 	// the keyboard event
		    text, 					// buffer when text will be written
		    10, 			        // size of the text buffer
		    &key, 					// workstation-independent key symbol
		    NULL);					// pointer to a composeStatus structure (unused)
		if (result == 1)
		{
			keyUpFunction(text[0], 0);
		}
		else
			keyUpFunction(((XKeyEvent *) &(dwi->evt))->keycode, 1);

	}
	else if ((dwi->evt).type == ClientMessage && (dwi->evt).xclient.data.l[0] == (unsigned int) dwi->wmDeleteMessage)
	{
		if (exitFunction)
			exitFunction();
		else
			exit(0);
	}
	else if ((dwi->evt).type == ConfigureNotify)
	{
		XConfigureEvent xce = (dwi->evt).xconfigure;

		if (xce.width != width || xce.height != height)
		{
			onResize(xce.width, xce.height);
		}

	}
}

void DrawWindow::onResize(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;

	//free current backbuffer
	XFreePixmap(dwi->dsp, dwi->backBuffer);

	//recreate backbuffer
	dwi->backBuffer = XCreatePixmap(dwi->dsp, dwi->win, width, height, 24);
}

int DrawWindow::numNextEvent()
{
	return XPending(dwi->dsp);
}

void DrawWindow::close()
{
	//XUnloadFont(dwi->dsp,dwi->font);

	XFreePixmap(dwi->dsp, dwi->backBuffer);
	XFreeGC(dwi->dsp, dwi->gc);

	XDestroyWindow(dwi->dsp, dwi->win);
	XCloseDisplay(dwi->dsp);

	free(dwi);
	dwi = 0;

	opened = false;
}

void DrawWindow::present()
{
	XCopyArea(dwi->dsp, dwi->backBuffer, dwi->win, dwi->gc, 0, 0, width, height, 0, 0);
	XFlush(dwi->dsp);
}

unsigned long DrawWindow::getTime()
{
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + (tv.tv_usec / 1000);
}

void DrawWindow::sleep(unsigned long time)
{
	usleep(time);
}

#endif
