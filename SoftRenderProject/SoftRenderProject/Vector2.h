#pragma once

class Matrix4x4;
class Vector2
{
public:
	float x,y,z;
	Vector2()
	{
		x = y = 0;		
		z = 1;
	};

	Vector2(float fx, float fy)
	{
		x = fx;
		y = fy;
		z = 1;
	}


	Vector2 operator + (const Vector2& right) const;
	Vector2 operator - (const Vector2& right) const;
	Vector2 operator * (float value) const;
	Vector2 operator / (float value) const;

	float SquareMagnitude();
	float Magnitude();
	void Normalize();
		
	Vector2 static Lerp(const Vector2& left, const Vector2& right, float t);

	void Print();
};
