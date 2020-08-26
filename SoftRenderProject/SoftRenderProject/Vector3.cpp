#include "framework.h"
#include "Vector3.h"
#include "math.h"
#include "stdio.h"
#include "Matrix4x4.h"
#include "Vector2.h"

Vector3 Vector3::operator+(const Vector3& right)const
{
	Vector3 ret(x + right.x, y + right.y, z + right.z, w + right.w);
	return ret;
}

Vector3 Vector3::operator-(const Vector3& right)const
{
	Vector3 ret(x - right.x, y - right.y, z - right.z, w - right.w);
	return ret;
}

Vector3 Vector3::operator * (float value) const
{
	Vector3 ret(x * value, y * value, z * value);
	return ret;
}

Vector3 Vector3::operator / (float value) const
{
	Vector3 ret(x / value, y / value, z / value);
	return ret;
}

Vector3 Vector3::operator-()const
{
	Vector3 ret(-x, -y, -z, w);
	return ret;
}

float Vector3::SquareMagnitude()
{
	return x*x + y*y + z*z;
}

float Vector3::Magnitude()
{
	return sqrt(SquareMagnitude());
}

void Vector3::Normalize()
{
	float len = Magnitude();
	x /= len;
	y /= len;
	z /= len;
}

float Vector3::Dot(const Vector3& left, const Vector3& right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z;
}

Vector3 Vector3::Cross(const Vector3& left, const Vector3& right)
{
	float valueX = left.y * right.z - left.z * right.y;
	float valueY = left.z * right.x - left.x * right.z;
	float valueZ = left.x * right.y - left.y * right.x;	
	return Vector3(valueX, valueY, valueZ, 0);
}

Vector3 Vector3::Lerp(const Vector3& from, const Vector3& to, float t)
{
	return to * t + from * (1 - t);
}

Vector3::operator Vector2()
{
	Vector2 ret(x,y);
	return ret;
}

void Vector3::Print()
{
	printf("(%f,%f,%f,%f)\n", x, y, z, w);
}
