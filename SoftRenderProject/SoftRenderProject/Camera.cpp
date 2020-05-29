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
	Vector3 lookDir = m_lookAt - m_position;
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
	t[3] = -m_position.x;
	t[7] = -m_position.y;
	t[11] = -m_position.z;

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
		s[0] = 2.0f / (_right - _left);
		s[5] = 2.0f / (_top - _bottom);
		s[10] = 2.0f / (_nearPlane - _farPlane);
		s[15] = 1.0f;
		
		Matrix4x4 t;
		t[3] = -(_right + _left) / 2.0f;
		t[7] = -(_top + _bottom) / 2.0f;
		t[11] = -(_farPlane + _nearPlane) / 2.0f;
		proj = s * t;
	}
	else
	{
		float tanValue = tan(0.5f * AngleToRad(_fov));
		proj[0] = 1.0f / (tanValue * _aspect);
		proj[5] = 1.0f / (tanValue);
		proj[10] = (_nearPlane + _farPlane) / (_nearPlane - _farPlane);
		proj[11] = -2*_nearPlane *_farPlane/(_nearPlane - _farPlane);
		proj[14] = 1.0f;
	}	
	return proj;

}

void Camera::SetOrthoCameraInfo(float size, float aspect, float nearPlane, float farPlane)
{
	_left = - size / aspect;
	_right = size / aspect;

	_top = size;
	_bottom = -size;

	_nearPlane = -nearPlane;
	_farPlane = -farPlane;
}

void Camera::SetPerspectiveCameraInfo(float fov, float aspect, float nearPlane, float farPlane)
{
	_fov = fov;
	_aspect = aspect;
	_nearPlane = -nearPlane;
	_farPlane = -farPlane;
}