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

#include "game/CPUPaddle.h"
#include "game/Ball.h"
#include "game/Game.h"
#include "game/Notification.h"
#include "engine/Bitmap.h"
#include "engine/DrawWindow.h"
#include "engine/Global.h"
#include "engine/ResourceLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <cmath>

CPUPaddle::CPUPaddle()
{
	x = global.window->width / 2;
	y = global.window->height * (2 / 5.0);
	vx = 500;
	length = 7;
	head = global.resource->getBitmap("cpuHead");
	middle = global.resource->getBitmap("cpuMiddle");
	end = global.resource->getBitmap("cpuEnd");
}

CPUPaddle::~CPUPaddle()
{

}

void CPUPaddle::render()
{
	global.window->drawBitmap(head, x - head->width, y);

	for (int i = 0; i < length; ++i)
		global.window->drawBitmap(middle, x + middle->width * i, y);

	global.window->drawBitmap(end, x + middle->width * length, y);
}

void CPUPaddle::checkBallIntersect()
{
	//if last hit was this object, then dont check
	if (game.ball->lastHitObject == this)
		return;

	//check to see if ball got hit
	float px = x - head->width;
	float py = y;
	float pw = head->width + middle->width * length + end->width;
	float ph = head->height;

	float distanceX = abs(game.ball->x - px - pw / 2);
	float distanceY = abs(game.ball->y - py - ph / 2);

	if (distanceX > (pw / 2 + game.ball->radius))
		return;

	if (distanceY > (ph / 2 + game.ball->radius))
		return;

	bool hit = false;

	if (distanceX <= (pw / 2))
	{
		game.ball->flipYDirection();
		hit = true;
	}
	else if (distanceY <= (ph / 2))
	{
		game.ball->flipXDirection();
		hit = true;
	}
	else
	{
		float cornerDistance = (distanceX - pw / 2) * (distanceX - pw / 2) + (distanceY - ph / 2) * (distanceY - ph / 2);

		bool cornerHit = (cornerDistance <= (game.ball->radius * game.ball->radius));

		if (cornerHit)
		{
			if (!game.ball->directionDetermined)
			{
				bool leftCornerHit = abs(game.ball->x - px) < abs(game.ball->x - (px + pw));
				bool topCornerHit = abs(game.ball->y - py) < abs(game.ball->y - (py + ph));

				if (leftCornerHit && game.ball->vx < 0)
				{
					game.ball->flipYDirection();
					hit = true;
				}

				else if (!leftCornerHit && game.ball->vx > 0)
				{
					game.ball->flipYDirection();
					hit = true;
				}
				else if (leftCornerHit && game.ball->vx > 0)
				{
					if (topCornerHit && game.ball->vy > 0)
						game.ball->flipXYDirection();
					else if (!topCornerHit && game.ball->vy < 0)
						game.ball->flipXYDirection();
					else
						game.ball->flipXDirection();

					hit = true;

				}
				else if (!leftCornerHit && game.ball->vx < 0)
				{
					if (topCornerHit && game.ball->vy > 0)
						game.ball->flipXYDirection();
					else if (!topCornerHit && game.ball->vy < 0)
						game.ball->flipXYDirection();
					else
						game.ball->flipXDirection();

					hit = true;
				}
			}
		}
	}

	if (hit)
	{
		if (game.ball->lastHitObject == (RenderObject*) (game.player))
		{
			game.score -= 100;
			game.notify->display("REJECTED! -100");
		}

		game.ball->lastHitObject = this;
	}

}

void CPUPaddle::update(unsigned int timeSinceLastFrame)
{

	checkBallIntersect();

	//move this back and forth
	if (x + middle->width * length + end->width >= global.window->width && vx > 0)
	{
		vx *= -1;
	}
	else if (x - head->width <= 0 && vx < 0)
	{
		vx *= -1;
	}

	float timeInSeconds = timeSinceLastFrame / 1000.0;
	x += timeInSeconds * vx;
}
