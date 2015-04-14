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

#include "game/Notification.h"

#include "engine/DrawWindow.h"
#include "engine/Global.h"
#include "game/Game.h"

void Notification::render()
{
	if (currentTime - startTime <= duration)
	{
		global.window->setForegroundColor(180, 0, 0);
		global.window->drawString(global.window->width * (1 / 3.0), global.window->height / 2, (char*) str.c_str());
	}
}

void Notification::update(unsigned int timeSinceLastFrame)
{
	this->currentTime = global.window->getTime();
}

void Notification::display(string inputStr)
{
	startTime = global.window->getTime();
	this->str = inputStr;
}
