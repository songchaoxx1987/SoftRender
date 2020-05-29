#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"


enum CAMERA_PROJECTION_MODE
{
	Perspective,
	Orthographic
};

class Camera
{
public:
	Camera();
	~Camera();

	Vector3 m_position;
	Vector3 m_lookAt;

	void SetOrthoCameraInfo(float size, float aspect, float near, float far);
	void SetPerspectiveCameraInfo(float fov, float aspect, float near, float far);

private:
	float _nearPlane;
	float _farPlane;
	// Perspective
	float _fov;
	float _aspect;	
	// Orthographic	
	float _left;
	float _right;
	float _top;
	float _bottom;

	CAMERA_PROJECTION_MODE mode;

	Matrix4x4 GetMatrix_Proj();
	Matrix4x4 GetMatrix_View();
private:
	Matrix4x4 projMatrix;
	Matrix4x4 viewMatrix;


};

