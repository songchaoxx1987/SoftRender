#include "framework.h"
#include "Vector2.h"
#include "math.h"
#include "stdio.h"
#include "Vector3.h"

Vector2 Vector2::operator+(const Vector2& right)const
{
	Vector2 ret(x + right.x, y + right.y);
	return ret;
}

Vector2 Vector2::operator-(const Vector2& right)const
{
	Vector2 ret(x - right.x, y - right.y);
	return ret;
}

Vector2 Vector2::operator * (float value) const
{
	Vector2 ret(x * value, y * value);
	return ret;
}

Vector2 Vector2::operator / (float value) const
{
	Vector2 ret(x / value, y / value);
	return ret;
}

float Vector2::Cross(const Vector2& left, const Vector2& right)
{
	return left.x * right.y - right.x * left.y;	
}

float Vector2::SquareMagnitude()
{
	return x*x + y*y;
}

float Vector2::Magnitude()
{
	return sqrt(SquareMagnitude());
}

void Vector2::Normalize()
{
	float len = Magnitude();
	x /= len;
	y /= len;
	z /= len;
}

void Vector2::Print()
{
	printf("(%f,%f,%f)\n", x, y, z);
}
