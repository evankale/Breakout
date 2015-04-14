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

#include "engine/Render.h"
#include "engine/RenderObject.h"
#include "engine/Global.h"
#include "engine/DrawWindow.h"

#include <algorithm>

Render::Render()
{
	FPS = 60;
}

Render::~Render()
{
	vector<RenderObject*>::iterator it;
	for (it = objects.begin(); it < objects.end(); it++)
	{
		RenderObject* object = *it;
		delete (object);
	}
}

void Render::start()
{
	unsigned long lastUpdateTime = global.window->getTime();
	unsigned long beginUpdateTime;
	unsigned long beginFrameTime;
	unsigned long totalFrameTime;
	unsigned long frameTimeMinimum;

	while (true)
	{
		beginFrameTime = global.window->getTime();

		while (global.window->numNextEvent() > 0)
		{
			global.window->doNextEvent();
		}

		beginUpdateTime = global.window->getTime();
		updateObjects(beginUpdateTime - lastUpdateTime);
		renderObjects();

		lastUpdateTime = beginUpdateTime;

		frameTimeMinimum = (unsigned int) 1000 / FPS;
		totalFrameTime = global.window->getTime() - beginFrameTime;

		if (totalFrameTime < frameTimeMinimum)
		{
			global.window->sleep(frameTimeMinimum - totalFrameTime);
		}
	}
}

void Render::renderObjects()
{
	global.window->clear();

	vector<RenderObject*>::iterator it;
	for (it = objects.begin(); it < objects.end();)
	{
		RenderObject* object = *it;

		if (object->markAsRemove)
		{
			removeObjects.push_back(object);
			//erase automatically increments iterator
			it = objects.erase(it);
		}
		else
		{
			object->render();
			++it;
		}
	}

	global.window->present();

	while (!removeObjects.empty())
	{
		RenderObject* object = removeObjects.back();
		removeObjects.pop_back();
		delete (object);
	}
}

void Render::updateObjects(unsigned int timeSinceLastFrame)
{
	vector<RenderObject*>::iterator it;
	for (it = objects.begin(); it < objects.end(); it++)
	{
		RenderObject* object = *it;
		object->update(timeSinceLastFrame);
	}
}

void Render::addObject(RenderObject * obj)
{
	objects.push_back(obj);
}

void Render::clear()
{
	vector<RenderObject*>::iterator it;
	for (it = objects.begin(); it < objects.end(); it++)
	{
		RenderObject* object = *it;
		object->markAsRemove = true;
	}
}
