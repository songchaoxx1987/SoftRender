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
	Vector3 worldNormal;
	Color color;
	Vector2 uv;	
		
	float rhw;

	static Vertex Lerp(const Vertex& v1, const Vertex& v2, float t);

	void BarycentricLerp(const Vertex& v1, const Vertex& v2, const Vertex& v3, float p1, float p2, float p3);
};

