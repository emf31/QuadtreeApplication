#pragma once
#include<utility>

class Circle
{
public:

	//Constructor of circle
	Circle(int id,int zbuffer,std::pair<int,int> center);

	//Destructor of circle
	~Circle();

	//Circle Id
	int Id;
	//Zbuffer (depth of circle)
	int Zbuffer;
	//radius of circle (make with define)
	const int Radius;
	//x,y of circle (make with std::pair)
	std::pair<int, int> Center;


};

