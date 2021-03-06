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

#ifndef GAME_H
#define GAME_H

#include <vector>

#define GAMESTATE_NONE 0
#define GAMESTATE_MENU 1
#define GAMESTATE_INGAME 2
#define GAMESTATE_GAMEOVER 3

using namespace std;

class Ball;
class PlayerPaddle;
class CPUPaddle;
class Brick;
class ScoreKeeper;
class Notification;
class Menu;
class GameOverScreen;

class Game
{
public:
	Ball * ball;
	PlayerPaddle * player;
	CPUPaddle * cpu;
	ScoreKeeper * scoreKeeper;
	Notification * notify;

	Menu * menu;
	GameOverScreen * gameOverScreen;

	int score;
	int multiplier;
	int gameState;
	bool isOver;
	int bricksRemaining;
	int currentLevel;

	//new game values:
	int newGame_frameRate;
	int newGame_initialBallSpeed;
	int newGame_initialPlayerSize;
	int newGame_initialEnemySize;
	int newGame_initialEnemySpeed;

	Game();
	~Game();

	void init();

	void gotoState(int state, int level);

private:
	void gotoMenuState();
	void gotoInGameState(int level);
	void gotoGameOverState();

};

extern Game game;

#endif
