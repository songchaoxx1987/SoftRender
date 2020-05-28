#include "framework.h"
#include "define.h"
#include "Camera.h"
#include "math.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}


Matrix4x4 Camera::GetMatrix_View()
{
	Vector3 lookDir = lookAt - position;
	lookDir.Normalize();

	Vector3 rightDir = Vector3::Cross(Vector3(0, 1, 0), lookDir);
	rightDir.Normalize();

	Vector3 upDir = Vector3::Cross(rightDir, lookDir);
	upDir.Normalize();

	Matrix4x4 m;	
	m[0] = rightDir.x; 
	m[1] = rightDir.y;
	m[2] = rightDir.z;
	m[3] = 0;
	
	m[4] = upDir.x;
	m[5] = upDir.y;
	m[6] = upDir.z;
	m[7] = 0;
	
	m[8] = -lookDir.x;
	m[9] = -lookDir.y;
	m[10] = -lookDir.z;
	m[11] = 0;

	m[12] = 0;
	m[13] = 0; 
	m[14] = 0;	   
	m[15] = 1;

	Matrix4x4 t;
	t[3] = -position.x;
	t[7] = -position.y;
	t[11] = -position.z;

	return m * t;
}

Matrix4x4 Camera::GetMatrix_Proj()
{
	//x,y,z->[-1,1]
	// ÓÒÊÖÏµ near > far
	Matrix4x4 proj;
	if (mode == Perspective)
	{
		Matrix4x4 s;
		s[0] = 2.0f / (right - left);
		s[5] = 2.0f / (top - bottom);
		s[10] = 2.0f / (nearPanel-farPanel);
		s[15] = 1.0f;
		
		Matrix4x4 t;
		t[3] = -(right + left) / 2.0f;
		t[7] = -(top + bottom) / 2.0f;
		t[11] = -(farPanel + nearPanel) / 2.0f;
		proj = s * t;
	}
	else
	{
		//float tanValue = tan(0.5f * AngleToRad(fov));
		//m.value[0] = 1.0f / (tanValue * aspect);
		//m.value[5] = 1.0f / (tanValue);
		//m.value[10] = farPanel / (farPanel - nearPanel);
		//m.value[11] = 1;
		//m.value[14] = -nearPanel * farPanel / (farPanel - nearPanel);
	}

	
	return proj;

}

