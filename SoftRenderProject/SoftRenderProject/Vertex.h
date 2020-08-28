#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Color.h"

class Vertex
{
public:
	Vector3 position;
	Vector3 normal;
	Vector3 worldPos;
	Color color;
	Vector2 uv;	
	Vector3 uv1;
	
	float rhw;
};

