#pragma once
#include "Color.h"
#include "Matrix4x4.h"
#include <string>

class Light;
class Camera;
class Scene;
class Texture;
//class Vector3;
class Vertex;

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

	static float zInShadowMap(float z)
	{
		//return 0.5 * z + 0.5;
		return 1.0 - 0.5 * (1.0 + z);
	}
	float AttenShadows(Vertex* v);
	Matrix4x4 m_lightVp;

	void SaveShadowMap2PNG(std::string file);
private:
	Camera* _pLightCamera;
	Texture* _pTexture;	
};

