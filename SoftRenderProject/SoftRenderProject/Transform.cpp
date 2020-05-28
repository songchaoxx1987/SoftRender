#include "define.h"
#include "Transform.h"
#include "stdio.h"
#include "math.h"

Transform::Transform()
{
	scale = 1;
}


Transform::~Transform()
{
}


Matrix4x4 Transform::Local2World()
{
	Matrix4x4 s;
	s.value[0] *= scale;
	s.value[5] *= scale;
	s.value[10] *= scale;
	
	Matrix4x4 mRot = RotX(euler.x) * RotY(euler.y) * RotZ(euler.z);
	Matrix4x4 m =  mRot * s;
	m.value[3] = position.x;
	m.value[7] = position.y;
	m.value[11] = position.z;

	return m;
}

Matrix4x4 Transform::World2Local()
{
	Matrix4x4 m = Local2World();
	m.Transpose();
	return m;
}

Matrix4x4 Transform::RotX(float angle)
{
	Matrix4x4 m;
	float sinV = sin(AngleToRad(angle));
	float cosV = cos(AngleToRad(angle));
	m.value[5] = cosV;
	m.value[6] = -sinV;
	m.value[9] = sinV;
	m.value[10] = cosV;	
	return m;
}

Matrix4x4 Transform::RotY(float angle)
{
	Matrix4x4 m;
	
	float sinV = sin(AngleToRad(angle));
	float cosV = cos(AngleToRad(angle));

	m.value[0] = cosV;
	m.value[2] = sinV;
	m.value[8] = -sinV;
	m.value[10] = cosV;
	return m;
}

Matrix4x4 Transform::RotZ(float angle)
{
	Matrix4x4 m;
	
	float sinV = sin(AngleToRad(angle));
	float cosV = cos(AngleToRad(angle));
	
	m.value[0] = cosV;
	m.value[1] = -sinV;
	m.value[4] = sinV;
	m.value[5] = cosV;
	return m;
}

