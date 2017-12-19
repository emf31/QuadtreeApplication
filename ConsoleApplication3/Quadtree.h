#pragma once
#include <vector>
#include <utility>
#include "Circle.h"

class Quadtree
{
public:
	Quadtree(std::pair<int, int> minXY, std::pair<int, int> maxXY, int depth);
	~Quadtree();

	Quadtree *q1;
	Quadtree *q2;
	Quadtree *q3;
	Quadtree *q4;
	int treeDepth;

	std::pair<int, int> MinXY;
	std::pair<int, int> MaxXY;
	std::vector<Circle> Objects;
	int Limit = 5;

	void insert(Circle* c);
	void split();
	bool isPointInside(std::pair<int, int> point);
	bool isCircleInside(Circle c);

	Circle* findByPoint(std::pair<int, int> point);
	std::vector<Circle*> getAllCirclesByPoint(std::pair<int, int> point);
};
