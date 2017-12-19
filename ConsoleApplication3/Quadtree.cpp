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
	if (!q1) {
		Objects.push_back(*c);
		if (Objects.size() == Limit) {
			split();
		}
	}
	else {
		if (q1->isPointInside(c->Center)) {
			q1->insert(c);
		}
		if (q2->isPointInside(c->Center)) {
			q2->insert(c);
		}
		if (q3->isPointInside(c->Center)) {
			q3->insert(c);
		}
		if (q4->isPointInside(c->Center)) {
			q4->insert(c);
		}
	}

	
}

void Quadtree::split()
{
	int mx, my, Mx, My, x2, y2;
	Mx = MaxXY.first;
	My = MaxXY.second;
	mx = MinXY.first;
	my = MinXY.second;
	
	x2 = (Mx + mx) / 2;
	y2 = (My + my) / 2;

	q1 = new Quadtree(MinXY,std::pair<int,int>(x2, y2),treeDepth+1);
	q2 = new Quadtree(std::pair<int, int>(x2, my), std::pair<int, int>(Mx, y2), treeDepth + 1);
	q3 = new Quadtree(std::pair<int, int>(mx, y2), std::pair<int, int>(x2, My), treeDepth + 1);
	q4 = new Quadtree(std::pair<int, int>(x2, y2), MaxXY, treeDepth + 1);

	al_draw_line(x2, my, x2, My, al_map_rgb(255 , 255, 255 ), 2);
	al_draw_line(mx, y2, Mx, y2, al_map_rgb(255, 255, 255 ), 2);

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
	if (point.first > MinXY.first && point.second > MinXY.second && point.first <= MaxXY.first && point.second <= MaxXY.second) {
		return true;
	}
	return false;
}

bool Quadtree::isCircleInside(Circle c)
{
	int dx = c.Center.first - std::max(MinXY.first, std::min(c.Center.first, MaxXY.first));
	int dy = c.Center.second - std::max(MinXY.second, std::min(c.Center.second, MaxXY.second));
	return ((dx * dx) + (dy * dy)) <= (c.Radius * c.Radius);
}

Circle * Quadtree::findByPoint(std::pair<int, int> point)
{
	std::vector<Circle*>  cs = getAllCirclesByPoint(point);
	Circle* c = nullptr;
	if (!cs.empty()) {
		c = cs.front();
		for (auto it = cs.begin(); it != cs.end(); ++it) {
			if ((*it)->Zbuffer < c->Zbuffer) {
				c = (*it);
			}
		}
		//std::cout << c->Id << std::endl;
	}
	
	return c;
}

std::vector<Circle*> Quadtree::getAllCirclesByPoint(std::pair<int, int> point)
{
	std::vector<Circle*> circles;
	std::vector<Circle*> childsVector;

	for (auto it = Objects.begin(); it != Objects.end(); ++it) {
		int vx = (*it).Center.first - point.first;
		int vy = (*it).Center.second - point.second;
			if (sqrt((vx*vx) + (vy*vy)) < (*it).Radius) {
				std::cout << "Circle Finded "<< (*it).Id << std::endl;
				circles.push_back(&(*it));
			}
	}
	
	if (q1 && q1->isPointInside(point))
		childsVector = q1->getAllCirclesByPoint(point);
	if (q2 && q2->isPointInside(point))
		childsVector = q2->getAllCirclesByPoint(point);
	if (q3 && q3->isPointInside(point))
		childsVector = q3->getAllCirclesByPoint(point);
	if (q4 && q4->isPointInside(point))
		childsVector = q4->getAllCirclesByPoint(point);

	//circles.insert(circles.end(), childsVector.begin(), childsVector.end());
	for (auto it = childsVector.begin(); it != childsVector.end(); ++it) {
		circles.push_back(*it);
	}

	return circles;
}
