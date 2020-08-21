#pragma once
#include "Transform.h"
#include "Color.h"
#include <list>

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
};

class RenderObject;
class Camera;

typedef std::list<RenderObject*> RENDER_LIST;
typedef std::list<Light*> LIGHT_LIST;
class Scene
{
public:
	Camera* m_pMainCamera;		
	RENDER_LIST m_renderObjects;
	LIGHT_LIST m_lights;

	void Init();
	void Release();
	void Update(float dt);
};

