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