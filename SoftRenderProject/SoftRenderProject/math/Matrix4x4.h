#pragma once

class Matrix4x4
{	
public:
	float value[16];
	Matrix4x4()
	{		
		memset(value, 0, 16*sizeof(float));	
	}

	float operator [] (const int idx) const;
	Matrix4x4 operator + (const Matrix4x4& right) const;
	Matrix4x4 operator - (const Matrix4x4& right) const;
	Matrix4x4 operator * (const Matrix4x4& right) const;
	Matrix4x4 operator * (float k) const;
	
	void Identity();
	void Transpose();

	void Print();
};