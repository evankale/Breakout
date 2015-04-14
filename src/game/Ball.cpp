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

#include "game/Ball.h"
#include "game/Game.h"
#include "game/PlayerPaddle.h"
#include "engine/Bitmap.h"
#include "engine/DrawWindow.h"
#include "engine/Global.h"
#include "engine/ResourceLoader.h"
#include "game/Notification.h"

Ball::Ball()
{
	x = -50;
	y = -50;
	vx = 0;
	vy = 0;
	radius = 18;
	bitmap = global.resource->getBitmap("ball");

	directionDetermined = false;
	w = bitmap->width;
	h = bitmap->height;

	lastHitObject = 0;
}

Ball::~Ball()
{

}

void Ball::render()
{
	global.window->drawBitmap(bitmap, x - bitmap->width / 2, y - bitmap->height / 2);
	directionDetermined = false;
}

void Ball::update(unsigned int timeSinceLastFrame)
{
	if (game.player->hasBall)
	{
		x = game.player->x;
		y = game.player->y - h / 2 - 1;
	}
	else
	{
		if (y >= global.window->height && vy > 0)
		{
			if (y > global.window->height + 200)
				game.gotoState(GAMESTATE_GAMEOVER, 0);

			if (!game.isOver)
			{
				game.notify->display("Slipped!! -500");
				game.score -= 500;
				game.isOver = true;
			}

		}
		else if (y - h / 2 <= 0 && vy < 0)
		{
			flipYDirection();
			lastHitObject = 0;
		}

		if (x + w / 2 >= global.window->width && vx > 0)
		{
			flipXDirection();
			lastHitObject = 0;
		}
		else if (x - w / 2 <= 0 && vx < 0)
		{
			flipXDirection();
			lastHitObject = 0;
		}

		float timeInSeconds = timeSinceLastFrame / 1000.0;
		x += timeInSeconds * vx;
		y += timeInSeconds * vy;
	}

}

void Ball::flipYDirection()
{
	if (!directionDetermined)
		vy *= -1;

	directionDetermined = true;
}

void Ball::flipXDirection()
{
	if (!directionDetermined)
		vx *= -1;

	directionDetermined = true;
}

void Ball::flipXYDirection()
{
	if (!directionDetermined)
	{
		vx *= -1;
		vy *= -1;
	}

	directionDetermined = true;
}
