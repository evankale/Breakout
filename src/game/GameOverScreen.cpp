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

#include "game/GameOverScreen.h"
#include "engine/Global.h"
#include "engine/ResourceLoader.h"
#include "engine/DrawWindow.h"
#include "engine/Bitmap.h"
#include "game/Game.h"

#include <stdio.h>

GameOverScreen::GameOverScreen()
{
	gameOverBmp = global.resource->getBitmap("gameover");
}

void GameOverScreen::render()
{
	global.window->drawBitmap(gameOverBmp, (global.window->width - gameOverBmp->width) / 2, 50);

	global.window->setForegroundColor(0, 0, 0);

	char scoreStr[50];
	sprintf(scoreStr, "Your Score : %i", game.score);

	global.window->drawString(global.window->width * (1 / 3.0), gameOverBmp->height + 200, scoreStr);
	global.window->drawString(global.window->width * (1 / 3.0), gameOverBmp->height + 300, (char*) "Press any key to continue");

}

void GameOverScreen::returnToMenu()
{
	game.gotoState(GAMESTATE_MENU, 0);
}
