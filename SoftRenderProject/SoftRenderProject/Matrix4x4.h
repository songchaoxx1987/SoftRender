#pragma once

class Vector3;
class Matrix4x4
{	
public:
	float value[16];
	Matrix4x4();	
	float& operator [] (const int idx);
	float operator [] (const int idx) const;
	Matrix4x4 operator + (const Matrix4x4& right) const;
	Matrix4x4 operator - (const Matrix4x4& right) const;
	Matrix4x4 operator * (const Matrix4x4& right) const;
	Matrix4x4 operator * (float k) const;
	Vector3 mul(const Vector3& v) const;
	
	void Identity();
	void Transpose();

	void Print();
};