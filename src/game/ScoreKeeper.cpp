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

#include "game/ScoreKeeper.h"

#include "engine/DrawWindow.h"
#include "engine/Global.h"
#include "game/Game.h"

#include <stdio.h>

void ScoreKeeper::render()
{
	global.window->setForegroundColor(0, 0, 0);

	char multiplierStr[50];
	sprintf(multiplierStr, "Multiplier: %ix", game.multiplier);

	char scoreStr[50];
	sprintf(scoreStr, "Score: %i", game.score);

	global.window->drawString(20, global.window->height - 125, multiplierStr);
	global.window->drawString(20, global.window->height - 100, scoreStr);
}
