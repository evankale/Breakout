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

#include "game/Item.h"
#include "game/Game.h"
#include "game/Ball.h"
#include "game/PlayerPaddle.h"
#include "game/CPUPaddle.h"
#include "game/Notification.h"
#include "engine/Bitmap.h"
#include "engine/DrawWindow.h"
#include "engine/Global.h"
#include "engine/ResourceLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

Item::Item()
{
	x = 0;
	y = 0;

	vy = 400;

	int itemType = rand() % 3;

	if (itemType == 0)
		bitmap = global.resource->getBitmap("item1");
	else if (itemType == 1)
		bitmap = global.resource->getBitmap("item2");
	else
		bitmap = global.resource->getBitmap("item3");

	w = bitmap->width;
	h = bitmap->height;
}

Item::~Item()
{

}

void Item::render()
{
	global.window->drawBitmap(bitmap, x, y);
}

void Item::checkPlayerIntersect()
{
	PlayerPaddle * player = game.player;

	bool intersect = !(x > player->x + player->middle->width * player->length + player->end->width || x + w < player->x - player->head->width || y > player->y + player->head->height || y + h < player->y);

	if (intersect)
	{
		markAsRemove = true;

		//increase multiplier
		game.multiplier++;

		//give random powerup:
		int randNum = 1 + rand() % 100;

		if (1 <= randNum && randNum <= 55)
		{
			//increase ball speed
			int speedInc = 40;

			if (game.ball->vx > 0)
				game.ball->vx += speedInc;
			else
				game.ball->vx -= speedInc;

			if (game.ball->vy > 0)
				game.ball->vy += speedInc;
			else
				game.ball->vy -= speedInc;

			game.notify->display("BALL speed up!!");
		}
		else if (56 <= randNum && randNum <= 70)
		{
			//increase cpu speed
			int speedInc = 50;
			if (game.cpu->vx > 0)
				game.cpu->vx += speedInc;
			else
				game.cpu->vx -= speedInc;

			game.notify->display("ENEMY speed up!!");
		}
		else if (71 <= randNum && randNum <= 85)
		{
			//increase cpu length
			game.cpu->length++;

			game.notify->display("ENEMY power up!!");
		}
		else
		{
			//increase player length
			game.player->length++;
			game.notify->display("PLAYER power up!!");
		}
	}
}

void Item::update(unsigned int timeSinceLastFrame)
{
	if (y >= global.window->height && vy > 0)
	{
		markAsRemove = true;
	}

	if (!markAsRemove)
	{
		//check against player paddle
		checkPlayerIntersect();

		float timeInSeconds = timeSinceLastFrame / 1000.0;
		y += timeInSeconds * vy;
	}

}
