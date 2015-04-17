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

#include "game/PlayerPaddle.h"
#include "game/Ball.h"
#include "game/Game.h"
#include "engine/Bitmap.h"
#include "engine/DrawWindow.h"
#include "engine/Global.h"
#include "engine/ResourceLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <cmath>

PlayerPaddle::PlayerPaddle()
{
	x = global.window->width / 2;
	y = global.window->height - 60;
	vx = 0;
	fireSpeed = 500;
	length = 7;
	head = global.resource->getBitmap("playerHead");
	middle = global.resource->getBitmap("playerMiddle");
	end = global.resource->getBitmap("playerEnd");

	hasBall = true;
}

PlayerPaddle::~PlayerPaddle()
{

}

void PlayerPaddle::render()
{
	global.window->drawBitmap(head, x - head->width, y);

	for (int i = 0; i < length; ++i)
		global.window->drawBitmap(middle, x + middle->width * i, y);

	global.window->drawBitmap(end, x + middle->width * length, y);

	lastX = x;
}

#define TO_DEGREES 57.295779513082320876798154814105f
#define TO_RADIANS 0.01745329251994329576923690768489f

void PlayerPaddle::checkBallIntersect()
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

	if (distanceX <= (pw / 2))
	{
		game.ball->flipYDirection();
		game.ball->lastHitObject = this;

		//alter the angel based on speed:
		float speed = sqrt(game.ball->vx * game.ball->vx + game.ball->vy * game.ball->vy);
		float angle = atan2(game.ball->vy, game.ball->vx) * TO_DEGREES + 90;
		//printf("angle = %f\n",angle);
		//printf("speed = %f\n",calculatedSpeed);
		angle -= calculatedSpeed * 10;

		if (angle <= -50)
			angle = -50;
		else if (angle >= 50)
			angle = 50;
		else if (angle == 0)
			angle = 0.001;

		angle -= 90;
		angle *= TO_RADIANS;

		game.ball->vx = speed * cos(angle);
		game.ball->vy = speed * sin(angle);

		return;
	}
	if (distanceY <= (ph / 2))
	{
		game.ball->flipXDirection();
		game.ball->lastHitObject = this;
		return;
	}

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

		return;
	}
}

void PlayerPaddle::update(unsigned int timeSinceLastFrame)
{
	if (vx != 0)
	{
		float newPos = x + vx;
		if (0 <= newPos && newPos <= global.window->width)
		{
			x = newPos;
		}
	}

	calculatedSpeed = (x - lastX) / timeSinceLastFrame;

	//printf("%f %f\n",lastX, x);

	checkBallIntersect();
}

#define TO_RADIAN 0.017453292519943295769236907684886f

void PlayerPaddle::fireBall()
{
	float speed = fireSpeed;
	float angle = (30 + rand() % 51) * TO_RADIAN;

	float ballvx = speed * cos(angle);
	float ballvy = -speed * sin(angle);

	if (rand() % 2 == 0)
		ballvx *= -1;

	game.ball->vx = ballvx;
	game.ball->vy = ballvy;
	game.ball->lastHitObject = this;
	hasBall = false;
}
