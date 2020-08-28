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
	if (!_dirtyFlag)
		return viewMatrix;
	Vector3 lookDir = m_lookAt - m_position;
	lookDir.Normalize();

	Vector3 rightDir = Vector3::Cross(lookDir, Vector3(0, 1, 0));
	rightDir.Normalize();

	Vector3 upDir = Vector3::Cross(rightDir, lookDir);
	upDir.Normalize();
	 
	float tx = -Vector3::Dot(rightDir, m_position);
	float ty = -Vector3::Dot(upDir, m_position);
	float tz = Vector3::Dot(lookDir, m_position);
	Matrix4x4 m;	
	m[0] = rightDir.x; 
	m[1] = rightDir.y;
	m[2] = rightDir.z;
	m[3] = tx;
	
	m[4] = upDir.x;
	m[5] = upDir.y;
	m[6] = upDir.z;
	m[7] = ty;
	
	m[8] = -lookDir.x;
	m[9] = -lookDir.y;
	m[10] = -lookDir.z;
	m[11] = tz;

	m[12] = 0;
	m[13] = 0; 
	m[14] = 0;	   
	m[15] = 1;

	viewMatrix = m;
	return m;	
}

Matrix4x4 Camera::GetMatrix_Proj()
{
	if (!_dirtyFlag)
		return projMatrix;
	//x,y,z->[-1,1]
	// 右手系 near > far
	Matrix4x4 proj;
	if (mode == Orthographic)
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
		proj[0] = -1.0f / (tanValue * _aspect);	// 因为是摄像机是朝向-z，所以这里有负号
		proj[5] = -1.0f / (tanValue);
		proj[10] = -(_nearPlane + _farPlane) / (_nearPlane - _farPlane);
		proj[11] = -2*_nearPlane *_farPlane/(_nearPlane - _farPlane);
		proj[14] = 1.0f;
		proj[15] = 0.0f;
	}	
	projMatrix = proj;
	return proj;

}

Vector3* Camera::GetWorldCorner()
{
	if (!_dirtyFlag)
		return _worldCorner;
	Vector3 lookDir = m_lookAt - m_position;
	lookDir.Normalize();

	Vector3 rightDir = Vector3::Cross(lookDir, Vector3(0, 1, 0));
	rightDir.Normalize();

	Vector3 upDir = Vector3::Cross(rightDir, lookDir);
	upDir.Normalize();

	Matrix4x4 m;
	m[0] = rightDir.x;
	m[1] = upDir.x;
	m[2] = -lookDir.x;
	m[3] = m_position.x;

	m[4] = rightDir.y;
	m[5] = upDir.y;
	m[6] = -lookDir.y;
	m[7] = m_position.y;

	m[8] = rightDir.z;
	m[9] = upDir.z;
	m[10] = -lookDir.z;
	m[11] = m_position.z;
		
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;
	
	float tanValue = tan(0.5f * AngleToRad(_fov));
	Vector3 nearPos;
	nearPos.z = -_nearPlane;
	nearPos.y = _nearPlane * tanValue;
	nearPos.x = nearPos.y * _aspect;

	Vector3 farPos;
	farPos.z = -_farPlane;
	farPos.y = _farPlane * tanValue;
	farPos.x = farPos.y * _aspect;
		
	_worldCorner[0].x = nearPos.x;
	_worldCorner[0].y = nearPos.y;
	_worldCorner[0].z = nearPos.z;

	_worldCorner[1].x = -nearPos.x;
	_worldCorner[1].y = nearPos.y;
	_worldCorner[1].z = nearPos.z;

	_worldCorner[2].x = -nearPos.x;
	_worldCorner[2].y = -nearPos.y;
	_worldCorner[2].z = nearPos.z;

	_worldCorner[3].x = nearPos.x;
	_worldCorner[3].y = -nearPos.y;
	_worldCorner[3].z = nearPos.z;

	_worldCorner[4].x = farPos.x;
	_worldCorner[4].y = farPos.y;
	_worldCorner[4].z = farPos.z;

	_worldCorner[5].x = -farPos.x;
	_worldCorner[5].y = farPos.y;
	_worldCorner[5].z = farPos.z;

	_worldCorner[6].x = -farPos.x;
	_worldCorner[6].y = -farPos.y;
	_worldCorner[6].z = farPos.z;

	_worldCorner[7].x = farPos.x;
	_worldCorner[7].y = -farPos.y;
	_worldCorner[7].z = farPos.z;

	return _worldCorner;
}

void Camera::SetOrthoCameraInfo(float size, float aspect, float nearPlane, float farPlane)
{
	mode = CAMERA_PROJECTION_MODE::Orthographic;
	_left = - size / aspect;
	_right = size / aspect;

	_top = size;
	_bottom = -size;

	_nearPlane = -nearPlane;
	_farPlane = -farPlane;
	_dirtyFlag = true;
}

void Camera::SetPerspectiveCameraInfo(float fov, float aspect, float nearPlane, float farPlane)
{
	mode = CAMERA_PROJECTION_MODE::Perspective;
	_fov = fov;
	_aspect = aspect;
	_nearPlane = nearPlane;
	_farPlane = farPlane;
	_dirtyFlag = true;
}

void Camera::BeforeRender()
{
	if (!_dirtyFlag)
		return;
	GetMatrix_Proj();
	GetMatrix_View();
	GetWorldCorner();
	_dirtyFlag = false;
}