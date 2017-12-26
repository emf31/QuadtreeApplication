#pragma once
#include <vector>
#include <utility>
#include "Circle.h"

class Quadtree
{
public:
	//Costructor of quadtree, it needs topleft and botright points of box quad (also need depth of the tree)
	Quadtree(std::pair<int, int> minXY, std::pair<int, int> maxXY, int depth);
	//Destructor of quadtree
	~Quadtree();

	//4 variables quadtree (childs of the tree)
	Quadtree *q1;
	Quadtree *q2;
	Quadtree *q3;
	Quadtree *q4;

	//Current depth
	int treeDepth;

	//Point top left
	std::pair<int, int> MinXY;
	//Point bot right
	std::pair<int, int> MaxXY;
	//Vector of circles
	std::vector<Circle> Objects;
	//Max circles per quad
	int Limit = 5;

	//insert a circle on the tree (this method can split the current tree)
	void insert(Circle* c);
	//Split the tree and the circles goes inside it
	void split();
	//Returns if a point is inside the current quad
	bool isPointInside(std::pair<int, int> point);
	//Returns if a circle or part of circle is inside the current quad
	bool isCircleInside(Circle c);

	//Finds the circle with less depth to show as solution (it use getAllCirclesByPoint)
	Circle* findByPoint(std::pair<int, int> point);
	//Returs a vector of circles founded on the quadtree that collision with the point
	std::vector<Circle*> getAllCirclesByPoint(std::pair<int, int> point);
};
