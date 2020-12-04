#pragma once
#include "Vertex.h"

//#define CHECKINSIDE(a,b,p)

class Trangle
{
public:
	Vertex v[3];	
	Vector2 bounds[2]; // 0 lt 1 rb
	float areaDouble;
	void calcBounds();		
};
