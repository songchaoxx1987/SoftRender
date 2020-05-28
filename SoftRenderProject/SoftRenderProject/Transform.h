#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

class Transform
{
public:
	Transform();
	~Transform();
	Vector3 position;
	Vector3 euler;
	float scale;
		
	Matrix4x4 Local2World();
	Matrix4x4 World2Local();

	static Matrix4x4 RotX(float angle);
	static Matrix4x4 RotY(float angle);
	static Matrix4x4 RotZ(float angle);

};

