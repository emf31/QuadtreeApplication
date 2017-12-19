#pragma once
#include<utility>

class Circle
{
public:
	Circle(int id,int zbuffer,std::pair<int,int> center);
	~Circle();
	int Id;
	int Zbuffer;
	const int Radius;
	std::pair<int, int> Center;


};

