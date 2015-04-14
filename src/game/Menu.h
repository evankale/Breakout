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

#ifndef MENU_H
#define MENU_H

#include "engine/RenderObject.h"

#include <string>

using namespace std;

class Bitmap;

class Menu: public RenderObject
{
public:
	Menu();
	~Menu();

	int numOptions;
	string options[5];
	int values[5];
	int maxValues[5];
	int minValues[5];

	virtual void render();

	int currentSelection;

	void menuUp();
	void menuDown();
	void menuLeft();
	void menuRight();
	void menuClick(int x, int y);
	void startGame();

	Bitmap * breakoutBmp;
	Bitmap * startBmp;

};

#endif
