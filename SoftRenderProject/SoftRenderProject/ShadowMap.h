#pragma once
#include "Color.h"
#include "Matrix4x4.h"

class Light;
class Camera;
class Scene;
class Texture;
class Vector3;

class ShadowMap
{
public:	
	ShadowMap();	
	~ShadowMap();

	Camera* InitLightCamera(Camera* pMainCamera, Light* pLight, Scene* pScene);	
	Texture* GetTexture() 
	{
		return _pTexture;	
	}

	Camera* GetCamera()
	{
		return _pLightCamera;
	}

	bool IsInShadows(Vector3* pWorldPos);
	Matrix4x4 m_lightVp;
private:
	Camera* _pLightCamera;
	Texture* _pTexture;	
};

