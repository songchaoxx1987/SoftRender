#pragma once

class Matrix4x4;
class Vector2;

class Vector3
{
public:
	float x, y, z, w;
	Vector3()
	{
		x = y = z = 0;
		w = 1;
	};
	Vector3(float fx, float fy, float fz)
	{
		x = fx;
		y = fy;
		z = fz;
		w = 1;
	}

	Vector3(float fx, float fy, float fz,float fw)
	{
		x = fx;
		y = fy;
		z = fz;
		w = fw;
	}
	operator Vector2();

	Vector3 operator + (const Vector3& right) const;
	Vector3 operator - (const Vector3& right) const;
	Vector3 operator * (float value) const;
	Vector3 operator / (float value) const;	
	Vector3 operator - () const;

	float SquareMagnitude();
	float Magnitude();
	void Normalize();
	
	float static Dot(const Vector3& left, const Vector3& right);
	Vector3 static Cross(const Vector3& left, const Vector3& right);
	Vector3 static Lerp(const Vector3& left, const Vector3& right, float t);
	Vector3 static Min(const Vector3& left, const Vector3& right);
	Vector3 static Max(const Vector3& left, const Vector3& right);

	void Print();
};
