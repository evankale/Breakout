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

#include "game/Menu.h"
#include "engine/Global.h"
#include "engine/ResourceLoader.h"
#include "engine/DrawWindow.h"
#include "engine/Bitmap.h"
#include "game/Game.h"

#include <stdio.h>

Menu::Menu()
{
	numOptions = 5;

	options[0] = "Frame rate : ";
	values[0] = game.newGame_frameRate;
	minValues[0] = 1;
	maxValues[0] = 120;

	options[1] = "Initial ball speed : ";
	values[1] = game.newGame_initialBallSpeed;
	minValues[1] = 100;
	maxValues[1] = 2000;

	options[2] = "Initial player size : ";
	values[2] = game.newGame_initialPlayerSize;
	minValues[2] = 1;
	maxValues[2] = 20;

	options[3] = "Initial enemy size : ";
	values[3] = game.newGame_initialEnemySize;
	minValues[3] = 1;
	maxValues[3] = 20;

	options[4] = "Initial enemy speed : ";
	values[4] = game.newGame_initialEnemySpeed;
	minValues[4] = 100;
	maxValues[4] = 2000;

	currentSelection = 0;

	breakoutBmp = global.resource->getBitmap("breakout");
	startBmp = global.resource->getBitmap("start");
}

Menu::~Menu()
{

}

void Menu::render()
{
	global.window->drawBitmap(breakoutBmp, (global.window->width - breakoutBmp->width) / 2, 50);
	global.window->drawBitmap(startBmp, (global.window->width - startBmp->width) / 2, global.window->height - 100);

	global.window->setForegroundColor(0, 0, 0);
	global.window->drawString(global.window->width * (1 / 3.0), breakoutBmp->height + 100, (char*) "Options (use arrow keys):");

	for (int i = 0; i < numOptions; ++i)
	{
		if (i == currentSelection)
			global.window->setForegroundColor(200, 0, 0);
		else
			global.window->setForegroundColor(0, 0, 0);

		char opt[100];
		sprintf(opt, "%s < %i >", (char*) options[i].c_str(), values[i]);
		global.window->drawString(global.window->width * (1 / 3.0), breakoutBmp->height + 120 + 20 * i, opt);
	}

	global.window->setForegroundColor(0, 0, 0);
	global.window->drawString(global.window->width * (1 / 3.0), breakoutBmp->height + 140 + 20 * numOptions, (char*) "(Window resize events are handled here)");
}

void Menu::menuUp()
{
	currentSelection--;
	if (currentSelection < 0)
		currentSelection = numOptions - 1;
}
void Menu::menuDown()
{
	currentSelection = (currentSelection + 1) % numOptions;
}
void Menu::menuLeft()
{
	if (values[currentSelection] > minValues[currentSelection])
		values[currentSelection]--;
}
void Menu::menuRight()
{
	if (values[currentSelection] < maxValues[currentSelection])
		values[currentSelection]++;
}
void Menu::menuClick(int x, int y)
{
	int startBtnX = (global.window->width - startBmp->width) / 2;
	int startBtnY = global.window->height - 100;

	if (startBtnX <= x && x <= startBtnX + startBmp->width && startBtnY <= y && y <= startBtnY + startBmp->height)
	{
		startGame();
	}
}
void Menu::startGame()
{
	game.newGame_frameRate = values[0];
	game.newGame_initialBallSpeed = values[1];
	game.newGame_initialPlayerSize = values[2];
	game.newGame_initialEnemySize = values[3];
	game.newGame_initialEnemySpeed = values[4];
	game.gotoState(GAMESTATE_INGAME, 1);
}
