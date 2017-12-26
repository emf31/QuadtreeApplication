#include "stdafx.h"
#include <iostream>
#include "Quadtree.h"
#include <algorithm>
#include <allegro5/allegro_primitives.h>


Quadtree::Quadtree(std::pair<int, int> minXY, std::pair<int, int> maxXY,int depth):q1(nullptr), q2(nullptr), q3(nullptr), q4(nullptr), MinXY(minXY), MaxXY(maxXY),treeDepth(depth)
{
}


Quadtree::~Quadtree()
{
	//Clears objects vector and delete their childs
	Objects.clear();
	if(q1)
	delete q1;
	if(q2)
	delete q2;
	if (q3)
	delete q3;
	if(q4)
	delete q4;

}

void Quadtree::insert(Circle* c)
{
	//If it didnt split (no exist any child) insert the circles inside himself
	if (!q1) {
		Objects.push_back(*c);
		//If limit is reached the quadtree needs to be splited
		if (Objects.size() > Limit) {
			split();
		}
	}
	//If is currently splited the quadtree, the circle is inserted in their childs (if is inside of them)
	else {
		if (q1->isCircleInside(*c)) {
			q1->insert(c);
		}
		if (q2->isCircleInside(*c)) {
			q2->insert(c);
		}
		if (q3->isCircleInside(*c)) {
			q3->insert(c);
		}
		if (q4->isCircleInside(*c)) {
			q4->insert(c);
		}
	}

	
}

void Quadtree::split()
{
	//Create variables to split (detect top-left(mx,my) and bot-right(Mx,My) and the point on the middle (x2,y2))
	int mx, my, Mx, My, x2, y2;
	Mx = MaxXY.first;
	My = MaxXY.second;
	mx = MinXY.first;
	my = MinXY.second;
	
	x2 = (Mx + mx) / 2;
	y2 = (My + my) / 2;

	//Making the 4 quadtrees with the positions and 1 depth more
	q1 = new Quadtree(MinXY,std::pair<int,int>(x2, y2),treeDepth+1);
	q2 = new Quadtree(std::pair<int, int>(x2, my), std::pair<int, int>(Mx, y2), treeDepth + 1);
	q3 = new Quadtree(std::pair<int, int>(mx, y2), std::pair<int, int>(x2, My), treeDepth + 1);
	q4 = new Quadtree(std::pair<int, int>(x2, y2), MaxXY, treeDepth + 1);

	//Drawing the lines to see the quadtree
	al_draw_line(x2, my, x2, My, al_map_rgb(255 , 255, 255 ), 1);
	al_draw_line(mx, y2, Mx, y2, al_map_rgb(255, 255, 255 ), 1);

	//Filling the childs quads with the circles of the father(only if the current quad is splited and the circle is inside)
	while (!Objects.empty()) {
		Circle c = Objects.back();
		if (q1 && q1->isCircleInside(c)) {
			q1->Objects.push_back(c);
		}
		if (q2 && q2->isCircleInside(c)) {
			q2->Objects.push_back(c);
		}
		if (q3 && q3->isCircleInside(c)) {
			q3->Objects.push_back(c);
		}
		if (q4 && q4->isCircleInside(c)) {
			q4->Objects.push_back(c);
		}
		Objects.pop_back();
	}

}

bool Quadtree::isPointInside(std::pair<int, int> point)
{
	//Returns true if the point is inside of the current quad
	if (point.first > MinXY.first && point.second > MinXY.second && point.first <= MaxXY.first && point.second <= MaxXY.second) {
		return true;
	}
	return false;
}

bool Quadtree::isCircleInside(Circle c)
{
	//Returns true if the point is inside of the current circle

	int dx = c.Center.first - std::max(MinXY.first, std::min(c.Center.first, MaxXY.first));
	int dy = c.Center.second - std::max(MinXY.second, std::min(c.Center.second, MaxXY.second));
	return ((dx * dx) + (dy * dy)) <= (c.Radius * c.Radius);
}

Circle * Quadtree::findByPoint(std::pair<int, int> point)
{
	//Get all the circles with collision in this point
	std::vector<Circle*>  cs = getAllCirclesByPoint(point);

	//Selecting the circle with less zbuffer
	Circle* c = nullptr;
	if (!cs.empty()) {
		c = cs.front();
		for (auto it = cs.begin(); it != cs.end(); ++it) {
			if ((*it)->Zbuffer < c->Zbuffer) {
				c = (*it);
			}
		}
	}
	
	return c;
}

std::vector<Circle*> Quadtree::getAllCirclesByPoint(std::pair<int, int> point)
{
	std::vector<Circle*> circles;
	std::vector<Circle*> childsVector;

	//Searching if in current quad are are any circle in collision whit point

	for (auto it = Objects.begin(); it != Objects.end(); ++it) {
		int vx = (*it).Center.first - point.first;
		int vy = (*it).Center.second - point.second;
			if (sqrt((vx*vx) + (vy*vy)) < (*it).Radius) {
				std::cout << "Circle Finded "<< (*it).Id << std::endl;
				circles.push_back(&(*it));
			}
	}
	
	//searching in childs if they have any circle in collision (only can enter in 1 of 4 childs)
	if (q1 && q1->isPointInside(point))
		childsVector = q1->getAllCirclesByPoint(point);
	if (q2 && q2->isPointInside(point))
		childsVector = q2->getAllCirclesByPoint(point);
	if (q3 && q3->isPointInside(point))
		childsVector = q3->getAllCirclesByPoint(point);
	if (q4 && q4->isPointInside(point))
		childsVector = q4->getAllCirclesByPoint(point);

	//Mergin current circles with childs circles to return it
	for (auto it = childsVector.begin(); it != childsVector.end(); ++it) {
		circles.push_back(*it);
	}

	return circles;
}
