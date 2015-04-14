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

#include "game/Brick.h"
#include "game/Ball.h"
#include "game/Game.h"
#include "game/Item.h"
#include "engine/Bitmap.h"
#include "engine/DrawWindow.h"
#include "engine/Global.h"
#include "engine/ResourceLoader.h"
#include "engine/Render.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

Brick::Brick()
{
	x = 0;
	y = 0;

	int brickType = rand() % 2;

	if (brickType == 0)
		bitmap = global.resource->getBitmap("brick");

	else
		bitmap = global.resource->getBitmap("brick2");

	w = bitmap->width;
	h = bitmap->height;
}

Brick::~Brick()
{

}

void Brick::render()
{
	global.window->drawBitmap(bitmap, x, y);
}

void Brick::update(unsigned int timeSinceLastFrame)
{
	float distanceX = abs(game.ball->x - x - w / 2);
	float distanceY = abs(game.ball->y - y - h / 2);

	if (distanceX > (w / 2 + game.ball->radius))
		return;

	if (distanceY > (h / 2 + game.ball->radius))
		return;

	if (distanceX <= (w / 2))
	{
		game.ball->flipYDirection();
		game.ball->lastHitObject = this;
		markAsRemove = true;
	}
	else if (distanceY <= (h / 2))
	{
		game.ball->flipXDirection();
		game.ball->lastHitObject = this;
		markAsRemove = true;
	}
	else
	{
		float cornerDistance = (distanceX - w / 2) * (distanceX - w / 2) + (distanceY - h / 2) * (distanceY - h / 2);

		bool cornerHit = (cornerDistance <= (game.ball->radius * game.ball->radius));

		if (cornerHit)
		{
			if (!game.ball->directionDetermined)
			{
				bool leftCornerHit = abs(game.ball->x - x) < abs(game.ball->x - (x + w));
				bool topCornerHit = abs(game.ball->y - y) < abs(game.ball->y - (y + h));

				if (leftCornerHit && game.ball->vx < 0)
				{
					game.ball->flipYDirection();
					game.ball->lastHitObject = this;
				}
				else if (!leftCornerHit && game.ball->vx > 0)
				{
					game.ball->flipYDirection();
					game.ball->lastHitObject = this;
				}
				else if (leftCornerHit && game.ball->vx > 0)
				{
					if (topCornerHit && game.ball->vy > 0)
						game.ball->flipXYDirection();
					else if (!topCornerHit && game.ball->vy < 0)
						game.ball->flipXYDirection();
					else
						game.ball->flipXDirection();

					game.ball->lastHitObject = this;
				}
				else if (!leftCornerHit && game.ball->vx < 0)
				{
					if (topCornerHit && game.ball->vy > 0)
						game.ball->flipXYDirection();
					else if (!topCornerHit && game.ball->vy < 0)
						game.ball->flipXYDirection();
					else
						game.ball->flipXDirection();

					game.ball->lastHitObject = this;
				}
			}

			markAsRemove = true;
		}
	}

	//if broken, add score, then spawn an item:
	if (markAsRemove)
	{
		game.score += 100 * game.multiplier;

		Item * item = new Item();
		item->x = x + w / 2;
		item->y = y + h / 2;
		global.render->addObject(item);

		game.bricksRemaining--;

		if (game.bricksRemaining == 0)
		{
			game.isOver = true;
			game.gotoState(GAMESTATE_GAMEOVER, 0);
		}
	}

}
