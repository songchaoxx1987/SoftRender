#include "../framework.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "math.h"
#include "stdio.h"


Matrix4x4 Matrix4x4::operator + (const Matrix4x4& right) const
{
	Matrix4x4 ret;
	for (int i = 0; i < 16; ++i)
	{
		ret.value[i] = value[i] + right[i];
	}	
	return ret;
}

Matrix4x4 Matrix4x4::operator - (const Matrix4x4& right) const
{
	Matrix4x4 ret;
	for (int i = 0; i < 16; ++i)
	{
		ret.value[i] = value[i] - right[i];
	}
	return ret;
}

Matrix4x4 Matrix4x4::operator * (const Matrix4x4& right) const
{
	Matrix4x4 ret;
	for (int i = 0; i < 4; ++i)
	{		
		for (int j = 0; j < 4; ++j)
		{
			ret.value[i*4 + j] = 0;
			for (int l = 0; l < 4; ++l)
			{				
				ret.value[i * 4 + j] += value[4 * i + l] * right[4 * l + j];
			}
		}
	}	
	return ret;
}

Matrix4x4 Matrix4x4::operator * (float k) const
{
	Matrix4x4 m;
	for (int i = 0; i < 16; ++i)
	{
		m.value[i] = value[i] * k;
	}
	return m;
}

Vector3 Matrix4x4::mul(const Vector3& v) const
{
	Vector3 ret;
	ret.x = value[0] * v.x + value[1] * v.y + value[2] * v.z + value[3] * v.w ;
	ret.y = value[4] * v.x + value[5] * v.y + value[6] * v.z + value[7] * v.w;
	ret.z = value[8] * v.x + value[9] * v.y + value[10] * v.z + value[11] * v.w;
	ret.w = value[12] * v.x + value[13] * v.y + value[14] * v.z + value[15] * v.w;
	return ret;
}

void Matrix4x4::Identity()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
				value[i*4 + j] = 1;
			else
				value[i * 4 + j] = 0;
		}
	}
}

void Matrix4x4::Transpose()
{
	float t = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < i; j++)
		{
			t = value[i * 4 + j];
			value[i * 4 + j] = value[j * 4 + i];
			value[j * 4 + i] = t;			
		}
	}
}

void Matrix4x4::Print()
{
	for (int i = 0; i < 16; ++i)
	{
		printf("%f\t", value[i]);
		if ((i + 1) % 4 == 0)
			printf("\n");
	}
}

float Matrix4x4::operator[](const int idx) const
{
	return value[idx];
}