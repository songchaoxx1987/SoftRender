#pragma once

class Matrix4x4;
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

	Vector3(float r, float g, float b, float a)
	{
		x = r;
		y = g;
		z = b;
		w = a;
	}


	Vector3 operator + (const Vector3& right) const;
	Vector3 operator - (const Vector3& right) const;
	Vector3 operator * (float value) const;
	Vector3 operator / (float value) const;
	Vector3 operator * (const Matrix4x4 &m);

	float SquareMagnitude();
	float Magnitude();
	void Normalize();
	Vector3 static MulAsColor(const Vector3 &left, const Vector3 &right);

	float static Dot(const Vector3& left, const Vector3& right);
	Vector3 static Cross(const Vector3& left, const Vector3& right);
	Vector3 static Lerp(const Vector3& left, const Vector3& right, float t);

	void Print();
};
