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

#ifndef BITMAP_H
#define BITMAP_H

#ifdef _X11
#include <X11/Xlib.h>
#elif _WIN32
#include <windows.h>
#endif

class DrawWindow;

class Bitmap
{
public:
	int width, height;

#ifdef _X11
	unsigned int * data;
	unsigned char * bitmask;
	XImage * ximage;
	Pixmap pixmap;
#elif _WIN32
	unsigned int * data;
	unsigned int * bitmask;
	HBITMAP hbmp, hbmp_mask;
	HDC hdcBmp, hdcBmp_mask;
#endif

	DrawWindow * drawWindow;

	Bitmap(const char * filename, DrawWindow * window);
	~Bitmap();

private:
	void alloc();
	bool isAlloced();
};

#endif
