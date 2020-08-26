#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Color.h"

class Vertex
{
public:
	Vector3 position;
	Vector3 normal;
	Color color;
	Vector2 uv;

	Vector3 uv1;
	Vector3 uv2;
	
	float rhw;
};

