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

#include "engine/Bitmap.h"
#include "engine/DrawWindow.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef _X11
#include <X11/Xutil.h>
#include <stdbool.h>
#endif

#define SWAP_WORD(word) (((unsigned short)(word) & 0x00ff) << 8) \
					  | (((unsigned short)(word) & 0xff00) >> 8)

#define SWAP_DWORD(dword) ((((unsigned int)(dword) & 0x000000ff) << 24) \
						 | (((unsigned int)(dword) & 0x0000ff00) <<  8) \
						 | (((unsigned int)(dword) & 0x00ff0000) >>  8) \
						 | (((unsigned int)(dword) & 0xff000000) >> 24))

#ifdef _BIGENDIAN
#define BITMAP_DWORD_SWAP( dword ) (dword) = SWAP_DWORD(dword);
#define BITMAP_WORD_SWAP(word)   (word)  = SWAP_WORD(word);

#else
#define BITMAP_DWORD_SWAP( dword )
#define BITMAP_WORD_SWAP(word)
#endif

#define PIXEL_ARGB(a, r, g, b) \
	(((a << 24)& 0xFF000000) | ((r << 16)& 0x00FF0000) | ((g << 8)& 0x0000FF00) | (b & 0x000000FF))

#define PIXEL_GETA(c) ((c & 0xFF000000) >> 24)
#define PIXEL_GETR(c) ((c & 0x00FF0000) >> 16)
#define PIXEL_GETG(c) ((c & 0x0000FF00) >> 8)
#define PIXEL_GETB(c)  (c & 0x000000FF)

struct BMPHead
{
	unsigned short type;
	unsigned int size;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned int offBits;
};

#define BITMAP_HEADER_SIZE 14

struct BMPInfo
{
	unsigned int size;
	int width;
	int height;
	unsigned short planes;
	unsigned short bitCount;
	unsigned int compression;
	unsigned int sizeImage;
	unsigned int xPelsPerMeter;
	unsigned int yPelsPerMeter;
	unsigned int clrUsed;
	unsigned int clrImportant;
};

#define BITMAP_INFO_SIZE 40

bool File_Getc(FILE * file, unsigned char * ch)
{
	int ich = fgetc(file);
	if (ich != EOF)
	{
		if (ch)
			*ch = (unsigned char) ich;
		return true;
	}
	else
		return false;
}

Bitmap::~Bitmap()
{
#ifdef _X11
	if (ximage)
	{
		XDestroyImage(ximage);
		//this also frees char * data
		data = 0;
	}
	else if (data)
	{
		free (data);
		data = 0;
	}

	if (pixmap)
	{
		XFreePixmap(drawWindow->dwi->dsp, pixmap); //this also frees char * bitmask
		bitmask = 0;
	}
	else if (bitmask)
	{
		free (bitmask);
		bitmask = 0;
	}
#elif _WIN32
	if(data)
	{
		free(data);
		data = 0;
	}
	if(bitmask)
	{
		free(bitmask);
		bitmask = 0;
	}
	if(hbmp)
	{
		DeleteObject(hbmp);
		hbmp = 0;
	}
	if(hbmp_mask)
	{
		DeleteObject(hbmp_mask);
		hbmp_mask = 0;
	}
	if(hdcBmp)
	{
		DeleteDC(hdcBmp);
		hdcBmp = 0;
	}
	if(hdcBmp_mask)
	{
		DeleteDC(hdcBmp_mask);
		hdcBmp_mask = 0;
	}
#endif
}

void Bitmap::alloc()
{
#ifdef _X11
	data = (unsigned int*) calloc(1, width * height * sizeof(unsigned int));
	bitmask = (unsigned char*) calloc(1, width * height);
#elif _WIN32
	data = (unsigned int*)calloc(1, width * height * sizeof(unsigned int));
	bitmask = (unsigned int*)calloc(1, width * height * sizeof(unsigned int));
#endif
}

bool Bitmap::isAlloced()
{
#ifdef _X11
	return data && bitmask;
#elif _WIN32
	return data && bitmask;
#endif
}

//only supports 24bpp bmps (and 32 for win32), and use 0xff00ff for transparent
Bitmap::Bitmap(const char * fileName, DrawWindow * window)
	: data(0), bitmask(0), width(0), height(0)
{
	memset(this, 0, sizeof(Bitmap));
	this->drawWindow = window;

	FILE *f = fopen(fileName, "rb");

	if (f)
	{
		struct BMPHead header;

		if (fread(&header, BITMAP_HEADER_SIZE, 1, f))
		{
			struct BMPInfo info;
			if (fread(&info, BITMAP_INFO_SIZE, 1, f))
			{

				BITMAP_DWORD_SWAP( header.offBits );
				BITMAP_DWORD_SWAP( info.height );
				BITMAP_DWORD_SWAP( info.width );
				BITMAP_DWORD_SWAP( info.clrUsed );
				BITMAP_WORD_SWAP( info.bitCount );

				if (info.bitCount == 24 || info.bitCount == 32)
				{
					width = info.width;
					height = info.height;
					alloc();
				}

				if (isAlloced())
				{
					int x, y;
					unsigned char div;
					unsigned char r, g, b, a;
					unsigned int pixel_argb;
					bool errorReading = false;

					if (info.bitCount == 24 || info.bitCount == 32)
					{
						unsigned int * dest24 = (unsigned int *) data + (info.height - 1) * width;
#ifdef _WIN32
						unsigned int * dest24_mask = (unsigned int *)bitmask + (info.height-1) * width;
#endif

						for (y = 0; y < info.height && !errorReading; y++)
						{
							for (x = 0; x < info.width; x++)
							{
								if (!File_Getc(f, &b))
								{
									errorReading = true;
									break;
								}
								if (!File_Getc(f, &g))
								{
									errorReading = true;
									break;
								}
								if (!File_Getc(f, &r))
								{
									errorReading = true;
									break;
								}
								if (info.bitCount == 32)
								{
									File_Getc(f, &a);
								}

#ifndef _BIGENDIAN
								*(dest24++) = PIXEL_ARGB(255,r,g,b);
#else
								*(dest24++) = PIXEL_ARGB(b,g,r,255);
#endif

								if (info.bitCount == 24)
								{
									pixel_argb = PIXEL_ARGB(0,r,g,b);

#ifdef _X11
									//write to bitmask:
									if (pixel_argb != PIXEL_ARGB(0,255,0,255) && y >= 0)
									{
										int bytesPerRow = (int) ceil(info.width / 8.0f);
										unsigned char * headOfRow = bitmask + (info.height - y - 1) * bytesPerRow;
										unsigned int byteNum = x / 8;
										unsigned int bitNum = x % 8;
										unsigned char currentByte = *(headOfRow + byteNum);
										currentByte ^= 1 << bitNum;
										*(headOfRow + byteNum) = currentByte;
									}
#elif _WIN32
									*(dest24_mask++) = (pixel_argb == PIXEL_ARGB(0,255,0,255))? 0x00000000 : 0xFFFFFFFF;
#endif
								}
								else if (info.bitCount == 32)
								{
#ifdef _X11
									//X11 currently does not support 32bpp - to support, copy from 24 bpp, modify if statement
#elif _WIN32
									*(dest24_mask++) = PIXEL_ARGB(a,a,a,a);
#endif
								}
							}
							if (!errorReading)
							{
								div = (info.width * 3) % 4;

								if (div)
									for (x = 0; x < 4 - div; x++)
										if (!File_Getc(f, NULL))
										{
											errorReading = true;
											break;
										}

								dest24 -= width * 2;
#ifdef _WIN32
								dest24_mask-=width*2;
#endif
							}
						}

						if (!errorReading)
						{
#ifdef _X11
							ximage = XCreateImage(window->dwi->dsp, window->dwi->visual, 24, ZPixmap, 0, (char*) data, width, height, 32, width * 4);
							pixmap = XCreatePixmapFromBitmapData(window->dwi->dsp, window->dwi->backBuffer, (char*) bitmask, width, height, 1, 0, 1);
#elif _WIN32
							hbmp = CreateBitmap(width,height,1,32,data);
							hbmp_mask = CreateBitmap(width,height,1,32,bitmask);

							hdcBmp = CreateCompatibleDC(window->dwi->hdc);
							hdcBmp_mask = CreateCompatibleDC(window->dwi->hdc);

							unsigned int * white = (unsigned int*)calloc(1, width * height * sizeof(unsigned int));
							memset(white,0xff,width * height * sizeof(unsigned int));
							HBITMAP hbmp_white = CreateBitmap(width,height,1,32,white);
							HDC hdcBmp_white = CreateCompatibleDC(window->dwi->hdc);

							SelectObject(hdcBmp, hbmp);
							SelectObject(hdcBmp_mask, hbmp_mask);
							SelectObject(hdcBmp_white, hbmp_white);
							BitBlt(hdcBmp, 0, 0, width, height, hdcBmp_mask, 0, 0, SRCAND);
							BitBlt(hdcBmp_mask, 0, 0, width, height, hdcBmp_white, 0, 0, SRCINVERT);

							DeleteObject(hbmp_white);
							DeleteDC(hdcBmp_white);
							free(white);
#endif
						}

					}

				}
			}
		}
		fclose(f);
	}
}

