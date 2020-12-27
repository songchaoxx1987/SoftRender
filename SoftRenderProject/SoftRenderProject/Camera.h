#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"


enum CAMERA_PROJECTION_MODE
{
	Perspective = 0,
	Orthographic
};

class FrameBuffer;
class Camera
{
public:
	Camera();
	~Camera();
	void SetPosition(const Vector3& position) 
	{ 
		m_position = position; 
		_dirtyFlag = true;
	}
	void SetLookAt(const Vector3& lookAt) 
	{
		m_lookAt = lookAt; 
		_dirtyFlag = true;
	}
	Vector3 Position() const
	{
		return m_position;
	}

	void SetOrthoCameraInfo(float size, float aspect, float near, float far);
	void SetPerspectiveCameraInfo(float fov, float aspect, float near, float far);
	Vector3* GetWorldCorner(float maxFar);
	const Matrix4x4& GetMatrix_Proj();
	const Matrix4x4& GetMatrix_View();
	Matrix4x4 GetMatrix_InvView();
	const Matrix4x4& GetMatrix_VP();

	void BeforeRender();
	
	void CreateFrameBuffer(int w, int h, int flag);
	FrameBuffer* GetFrameBuffer()
	{
		return _pFrameBuffer;
	}		

	CAMERA_PROJECTION_MODE Projection() { return mode; }

public:
	float Near() { return -_nearPlane; }
	float Far() { return -_farPlane; }
private:
	Vector3 m_position;
	Vector3 m_lookAt;

	float _nearPlane;
	float _farPlane;
	// Perspective
	float _fov;
	float _aspect;	
	// Orthographic	
	float _size;
	
	Vector3 _worldCorner[8];
	float _maxCornerFar;

	bool _dirtyFlag = false;

	CAMERA_PROJECTION_MODE mode;

	Matrix4x4 projMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 vpMatrix;
	FrameBuffer* _pFrameBuffer;

};

