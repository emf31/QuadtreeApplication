#include "stdafx.h"
#include "Circle.h"
#define radius 10


Circle::Circle(int id, int zbuffer, std::pair<int, int> center):Id(id),Zbuffer(zbuffer),Center(center),Radius(radius)
{
}

Circle::~Circle()
{
}
