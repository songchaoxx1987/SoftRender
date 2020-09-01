#pragma once
#include "Transform.h"
#include "Color.h"
#include "define.h"
#include <list>
#include <vector>
enum LightMode
{
	directLight=0,
	pointLight,
};

class Light
{
public:
	Transform transform;
	Color color;
	LightMode mode;
	bool castShadow;
	void CalcDir()
	{
		if (mode == LightMode::directLight)
			_dir = transform.Local2World().mul(Vector3(0, 0, 1, 0));		
		_dir.Normalize();
	}

	Vector3 Dir()
	{
		return _dir;
	}

	Vector3 InvDir()
	{
		return -_dir;
	}

private:
	Vector3 _dir;	
};

//class RenderObject;
class Camera;

//typedef std::list<RenderObject*> RENDER_LIST;
typedef std::vector<Light*> LIGHT_LIST;
class Scene
{
public:
	Camera* m_pMainCamera;		
	RENDER_LIST m_renderObjects;
	LIGHT_LIST m_lights;
	Color ambLight;
	void Init();
	void Release();
	void Update(float dt);
	void CalcBounds();

	Vector3 m_bounds[2];
};

