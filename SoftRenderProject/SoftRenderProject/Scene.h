#pragma once
#include "Transform.h"
#include "Color.h"
#include "define.h"
#include <list>
#include <vector>


//class RenderObject;
class Camera;
class Light;

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
	Vector3 m_corners[8];
};

