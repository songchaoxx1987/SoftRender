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

	Vector3 position;
	Vector3 lookAt;

	float nearPanel;
	float farPanel;
	// Perspective
	float fov;
	float aspect;	
	// Orthographic
	float size;
	float left;
	float right;
	float top;
	float bottom;

	CAMERA_PROJECTION_MODE mode;

	Matrix4x4 GetMatrix_Proj();
	Matrix4x4 GetMatrix_View();
private:
	Matrix4x4 projMatrix;
	Matrix4x4 viewMatrix;


};

