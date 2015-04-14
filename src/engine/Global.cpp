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

#include "engine/Global.h"
#include "engine/DrawWindow.h"
#include "engine/ResourceLoader.h"
#include "engine/Render.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Global global;

Global::Global()
{
	//initialize the window
	window = new DrawWindow();
	window->x = 500;
	window->y = 100;
	window->width = 800;
	window->height = 600;

	//open the window
	window->open();

	//empty window contents
	window->setBackgroundColor(217, 204, 172);
	window->setForegroundColor(255, 255, 255);
	window->clear();
	window->present();

	resource = new ResourceLoader();
	resource->loadResources();

	render = new Render();

	srand(global.window->getTime());

}

Global::~Global()
{
	delete (render);
	delete (resource);
	delete (window);
}

