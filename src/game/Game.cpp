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

#include "game/Game.h"
#include "game/PlayerPaddle.h"
#include "game/CPUPaddle.h"
#include "game/Brick.h"
#include "game/Ball.h"
#include "game/Notification.h"
#include "game/ScoreKeeper.h"
#include "game/Menu.h"
#include "game/GameOverScreen.h"

#include "engine/Global.h"
#include "engine/Render.h"
#include "engine/Bitmap.h"
#include "engine/DrawWindow.h"
#include "engine/ResourceLoader.h"

Game game;

Game::Game()
{
	score = 0;
	multiplier = 1;

	newGame_frameRate = 60;
	newGame_initialBallSpeed = 400;
	newGame_initialPlayerSize = 5;
	newGame_initialEnemySize = 4;
	newGame_initialEnemySpeed = 250;

	gameState = GAMESTATE_NONE;
}

Game::~Game()
{

}

void Game::init()
{
	global.window->setBackgroundBitmap(global.resource->getBitmap("bg"));

	gotoState(GAMESTATE_MENU, 0);

}

void Game::gotoState(int state, int level)
{
	global.render->clear();

	//goto the new game state
	switch (state)
	{
		case GAMESTATE_NONE:
			gameState = GAMESTATE_NONE;
		break;
		case GAMESTATE_MENU:
			gotoMenuState();
		break;
		case GAMESTATE_INGAME:
			gotoInGameState(level);
		break;
		case GAMESTATE_GAMEOVER:
			gotoGameOverState();
		break;
	}
}

void Game::gotoMenuState()
{
	global.render->FPS = 60;

	menu = new Menu();
	global.render->addObject(menu);

	gameState = GAMESTATE_MENU;
}

void Game::gotoInGameState(int level)
{

	global.render->FPS = newGame_frameRate;
	score = 0;
	multiplier = 1;
	bricksRemaining = 0;
	isOver = false;
	currentLevel = level;

	if (level == 1)
	{
		player = new PlayerPaddle();
		player->fireSpeed = newGame_initialBallSpeed;
		player->length = newGame_initialPlayerSize;
		global.render->addObject(player);

		cpu = new CPUPaddle();
		cpu->vx = newGame_initialEnemySpeed;
		cpu->length = newGame_initialEnemySize;
		global.render->addObject(cpu);

		int brickWidth = global.resource->getBitmap("brick")->width;
		int brickHeight = global.resource->getBitmap("brick")->height;
		int numX = global.window->width / brickWidth;
		int numY = (global.window->height * (2.0 / 5.0)) / brickHeight;

		for (int y = 0; y < numY; ++y)
		{
			for (int x = 0; x < numX; ++x)
			{
				Brick * brick = new Brick();
				brick->x = brick->bitmap->width * x;
				brick->y = brick->bitmap->height * y;
				if (y % 2 == 0)
					brick->x += brick->bitmap->width / 2;
				global.render->addObject(brick);
				bricksRemaining++;
			}
		}

		ball = new Ball();
		global.render->addObject(ball);

		scoreKeeper = new ScoreKeeper();
		global.render->addObject(scoreKeeper);

		notify = new Notification();
		notify->display("Level 1 - Click to begin! Hit 'n' for Level 2");
		global.render->addObject(notify);
	}
	else if (level == 2)
	{
		player = new PlayerPaddle();
		player->fireSpeed = newGame_initialBallSpeed;
		player->length = newGame_initialPlayerSize;
		global.render->addObject(player);

		int brickWidth = global.resource->getBitmap("brick")->width;
		int brickHeight = global.resource->getBitmap("brick")->height;
		int numX = global.window->width / brickWidth;
		int numY = (global.window->height * (4.0 / 5.0)) / brickHeight;

		for (int y = 0; y < numY; y++)
		{
			for (int x = 0; x < numX; x += 2)
			{
				Brick * brick = new Brick();
				brick->x = brick->bitmap->width * x;
				brick->y = brick->bitmap->height * y;
				if (y % 2 == 0)
					brick->x += brick->bitmap->width / 2;
				global.render->addObject(brick);
				bricksRemaining++;
			}
		}

		cpu = new CPUPaddle();
		cpu->vx = newGame_initialEnemySpeed;
		cpu->length = newGame_initialEnemySize;
		global.render->addObject(cpu);

		ball = new Ball();
		global.render->addObject(ball);

		scoreKeeper = new ScoreKeeper();
		global.render->addObject(scoreKeeper);

		notify = new Notification();
		notify->display("Level 2 - Click to begin! Hit 'n' for Level 3");
		global.render->addObject(notify);
	}

	else if (level == 3)
	{
		player = new PlayerPaddle();
		player->fireSpeed = newGame_initialBallSpeed;
		player->length = newGame_initialPlayerSize;
		global.render->addObject(player);

		int brickWidth = global.resource->getBitmap("brick")->width;
		int brickHeight = global.resource->getBitmap("brick")->height;
		int numX = global.window->width / brickWidth;
		int numY = (global.window->height * (4.0 / 5.0)) / brickHeight;

		for (int y = 0; y < numY; y++)
		{
			for (int x = 0; x < numX; x = x + ((y % 2 == 0) ? 1 : 2))
			{
				Brick * brick = new Brick();
				brick->x = brick->bitmap->width * x;
				brick->y = brick->bitmap->height * y;
				if (y % 2 == 0)
					brick->x += brick->bitmap->width / 2;
				global.render->addObject(brick);
				bricksRemaining++;
			}
		}

		cpu = new CPUPaddle();
		cpu->y = global.window->height * (4 / 6.0);
		cpu->vx = -newGame_initialEnemySpeed;
		cpu->length = newGame_initialEnemySize;
		global.render->addObject(cpu);

		cpu = new CPUPaddle();
		cpu->y = global.window->height * (2 / 6.0);
		cpu->vx = newGame_initialEnemySpeed;
		cpu->length = newGame_initialEnemySize;
		global.render->addObject(cpu);

		ball = new Ball();
		global.render->addObject(ball);

		scoreKeeper = new ScoreKeeper();
		global.render->addObject(scoreKeeper);

		notify = new Notification();
		notify->display("Level 3 - Click to begin! Hit 'n' for Level 1");
		global.render->addObject(notify);
	}

	gameState = GAMESTATE_INGAME;

}

void Game::gotoGameOverState()
{
	global.render->FPS = 60;

	gameOverScreen = new GameOverScreen();
	global.render->addObject(gameOverScreen);

	gameState = GAMESTATE_GAMEOVER;
}
