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

#ifndef RENDER_H
#define RENDER_H

#include <vector>

using namespace std;

class RenderObject;

class Render
{
public:
	void start();
	void addObject(RenderObject * obj);
	Render();
	~Render();

	int FPS;

	void clear();

private:
	vector<RenderObject*> objects;
	vector<RenderObject*> removeObjects;
	void renderObjects();
	void updateObjects(unsigned int timeSinceLastFrame);
};

#endif
