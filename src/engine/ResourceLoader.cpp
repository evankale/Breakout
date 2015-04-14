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

#include "engine/ResourceLoader.h"
#include "engine/Bitmap.h"
#include "engine/Global.h"

void ResourceLoader::loadResources()
{
	Bitmap * brickBitmap = new Bitmap("./res/brick.bmp", global.window);
	Bitmap * brick2Bitmap = new Bitmap("./res/brick2.bmp", global.window);
	Bitmap * ballBitmap = new Bitmap("./res/ball.bmp", global.window);
	Bitmap * bgBitmap = new Bitmap("./res/bg.bmp", global.window);
	Bitmap * playerHeadBitmap = new Bitmap("./res/player_left.bmp", global.window);
	Bitmap * playerMiddleBitmap = new Bitmap("./res/player_middle.bmp", global.window);
	Bitmap * playerEndBitmap = new Bitmap("./res/player_right.bmp", global.window);
	Bitmap * cpuHeadBitmap = new Bitmap("./res/cpu_left.bmp", global.window);
	Bitmap * cpuMiddleBitmap = new Bitmap("./res/cpu_middle.bmp", global.window);
	Bitmap * cpuEndBitmap = new Bitmap("./res/cpu_right.bmp", global.window);
	Bitmap * item1Bitmap = new Bitmap("./res/item1.bmp", global.window);
	Bitmap * item2Bitmap = new Bitmap("./res/item2.bmp", global.window);
	Bitmap * item3Bitmap = new Bitmap("./res/item3.bmp", global.window);
	Bitmap * breakoutBitmap = new Bitmap("./res/breakout.bmp", global.window);
	Bitmap * gameoverBitmap = new Bitmap("./res/gameover.bmp", global.window);
	Bitmap * startBitmap = new Bitmap("./res/start.bmp", global.window);

	bitmaps.insert(pair<string, Bitmap*>("brick", brickBitmap));
	bitmaps.insert(pair<string, Bitmap*>("brick2", brick2Bitmap));
	bitmaps.insert(pair<string, Bitmap*>("ball", ballBitmap));
	bitmaps.insert(pair<string, Bitmap*>("bg", bgBitmap));
	bitmaps.insert(pair<string, Bitmap*>("playerHead", playerHeadBitmap));
	bitmaps.insert(pair<string, Bitmap*>("playerMiddle", playerMiddleBitmap));
	bitmaps.insert(pair<string, Bitmap*>("playerEnd", playerEndBitmap));
	bitmaps.insert(pair<string, Bitmap*>("cpuHead", cpuHeadBitmap));
	bitmaps.insert(pair<string, Bitmap*>("cpuMiddle", cpuMiddleBitmap));
	bitmaps.insert(pair<string, Bitmap*>("cpuEnd", cpuEndBitmap));
	bitmaps.insert(pair<string, Bitmap*>("item1", item1Bitmap));
	bitmaps.insert(pair<string, Bitmap*>("item2", item2Bitmap));
	bitmaps.insert(pair<string, Bitmap*>("item3", item3Bitmap));
	bitmaps.insert(pair<string, Bitmap*>("breakout", breakoutBitmap));
	bitmaps.insert(pair<string, Bitmap*>("gameover", gameoverBitmap));
	bitmaps.insert(pair<string, Bitmap*>("start", startBitmap));

}

Bitmap * ResourceLoader::getBitmap(string id)
{
	return bitmaps[id];
}

ResourceLoader::ResourceLoader()
{

}

ResourceLoader::~ResourceLoader()
{
	map<string, Bitmap*>::iterator it;
	for (it = bitmaps.begin(); it != bitmaps.end(); it++)
	{
		Bitmap * bitmap = (*it).second;
		delete (bitmap);
	}
}
