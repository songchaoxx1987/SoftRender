#pragma once
#include "define.h"
#include "Matrix4x4.h"
#include <string>

class Light;
class Camera;
class Scene;
class Vertex;

class ShadowMap
{
public:	
	ShadowMap();	
	~ShadowMap();

	Camera* InitLightCamera(Camera* pMainCamera, Light* pLight, Scene* pScene);	
	Camera* GetCamera()
	{
		return _pLightCamera;
	}

	static float zInShadowMap(float z)
	{
		z = Clamp<float>(z, -1.0f, 1.0f);
		//return 0.5 * z + 0.5;
		return 1.0 - 0.5 * (1.0 + z);
	}
	float AttenShadow(Vertex* v);
	Matrix4x4 m_lightVp;

	void SaveShadowMap2PNG(std::string file);
private:
	Camera* _pLightCamera;	
};

