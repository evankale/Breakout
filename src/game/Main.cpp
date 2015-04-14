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

#include <stdlib.h>
#include <stdio.h>

#ifdef _LINUX
#include <stdbool.h>
#endif

#include "engine/Global.h"
#include "engine/Render.h"
#include "engine/DrawWindow.h"
#include "game/Game.h"
#include "game/PlayerPaddle.h"
#include "game/Menu.h"
#include "game/GameOverScreen.h"

using namespace std;

void onMouseMove(int x, int y);
void onMouseDown(int x, int y);
void onKeyDown(char key, bool isKeyCode);
void onKeyUp(char key, bool isKeyCode);

int main()
{
	global.window->mouseMoveFunction = onMouseMove;
	global.window->mouseDownFunction = onMouseDown;
	global.window->keyDownFunction = onKeyDown;
	global.window->keyUpFunction = onKeyUp;

	game.init();

	global.render->start();

	return 0;
}

//mouse clicked on x,y
void onMouseMove(int x, int y)
{
	if (x == 0 && y == 0)
		return;

	if (game.gameState == GAMESTATE_INGAME)
	{
		game.player->x = x;
	}

}

void onMouseDown(int x, int y)
{
	if (x == 0 && y == 0)
		return;

	if (game.gameState == GAMESTATE_INGAME)
	{
		if (game.player->hasBall)
			game.player->fireBall();
	}

	else if (game.gameState == GAMESTATE_MENU)
	{
		game.menu->menuClick(x, y);
	}
	else if (game.gameState == GAMESTATE_GAMEOVER)
	{
		game.gameOverScreen->returnToMenu();
	}

}

void onKeyDown(char key, bool isKeyCode)
{
	if (key == 0)
		return;

	if (game.gameState == GAMESTATE_INGAME)
	{
		if (!isKeyCode)
		{
			switch (key)
			{
				case 'N':
					if (game.currentLevel == 1)
						game.gotoState(GAMESTATE_INGAME, 2);
					else if (game.currentLevel == 2)
						game.gotoState(GAMESTATE_INGAME, 3);
					else if (game.currentLevel == 3)
						game.gotoState(GAMESTATE_INGAME, 1);
				break;
				case 'A':
					game.player->vx = -20;
				break;
				case 'D':
					game.player->vx = 20;
				break;
			}
		}

	}
	else if (game.gameState == GAMESTATE_MENU)
	{
		if (isKeyCode)
		{
			switch (key)
			{
				case KEY_UP:
					game.menu->menuUp();
				break;
				case KEY_DOWN:
					game.menu->menuDown();
				break;
				case KEY_LEFT:
					game.menu->menuLeft();
				break;
				case KEY_RIGHT:
					game.menu->menuRight();
				break;
				case KEY_RETURN:
					game.menu->startGame();
				break;
			}
		}
	}
	else if (game.gameState == GAMESTATE_GAMEOVER)
	{
		game.gameOverScreen->returnToMenu();
	}

}

void onKeyUp(char key, bool isKeyCode)
{
	if (key == 0)
		return;

	if (game.gameState == GAMESTATE_INGAME)
	{
		if (!isKeyCode)
		{
			switch (key)
			{
				case 'A':
					game.player->vx = 0;
				break;
				case 'D':
					game.player->vx = 0;
				break;
			}
		}

	}
}
