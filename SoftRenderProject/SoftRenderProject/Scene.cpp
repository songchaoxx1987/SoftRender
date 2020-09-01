#include "Scene.h"
#include "define.h"
#include "Camera.h"
#include "RenderObject.h"
#include "CDevice.h"

extern CDevice* pDevice;

void Scene::Init()
{
	m_pMainCamera = new Camera();
	m_pMainCamera->SetPosition(Vector3(0, 5, 5));
	m_pMainCamera->SetLookAt(Vector3(0, 4, 4));
	//m_pMainCamera->m_position = Vector3(0, 0, 5);
	//m_pMainCamera->m_lookAt = Vector3(0, 0, -4);
	m_pMainCamera->SetPerspectiveCameraInfo(60, (float)pDevice->screenWidth / (float)pDevice->screenHeight, 0.1f, 1000);
	//m_pMainCamera->SetOrthoCameraInfo(5, (float)pDevice->screenWidth / (float)pDevice->screenHeight, 0.1f, 1000);

	RenderObject* pObj = NULL;
	pObj = new RenderObject();	
	pObj->Create("Cube", "m02");
	pObj->enableMove = true;
	pObj->m_transform.position = Vector3(0, 0, 0);
	//pObj->m_transform.euler = Vector3(0, 30, 0);
	m_renderObjects.push_back(pObj);


	pObj = new RenderObject();
	pObj->Create("Plane", "m03");
	m_renderObjects.push_back(pObj);
	pObj->m_transform.position = Vector3(0.5, -1, 0);
	pObj->m_transform.scale *= 6;
	//pObj->m_transform.euler = Vector3(0, 0, 0);

	pObj = new RenderObject();
	pObj->Create("Cube", "m01");
	m_renderObjects.push_back(pObj);
	pObj->m_transform.position = Vector3(1.5, 0, 0);
	
	pObj = new RenderObject();
	pObj->Create("cow", "cow");
	pObj->m_transform.position = Vector3(-1.5, 0, 0);
	pObj->m_transform.euler = Vector3(0, 180, 0);
	//pObj->enableMove = true;	
	m_renderObjects.push_back(pObj);

	pObj = new RenderObject();
	pObj->Create("cow", "cow2");
	pObj->m_transform.position = Vector3(-1.5, 0, 2);
	pObj->m_transform.euler = Vector3(0, 180, 0);
	//pObj->enableMove = true;	
	m_renderObjects.push_back(pObj);

	Light* pLight = new Light();
	pLight->transform.position = Vector3(0, 0, 0);
	//pLight->transform.euler = Vector3(0, 180, 0);
	pLight->transform.euler = Vector3(45, 45, 0);
	pLight->mode = LightMode::directLight;
	pLight->color = Color(0.3f, 0.37f, 0.51f);
	pLight->castShadow = true;
	pLight->CalcDir();

	m_lights.push_back(pLight);

	ambLight = Color(0.2f, 0.2f, 0.2f);
}

void Scene::Release()
{	
	for (RENDER_LIST::iterator it = m_renderObjects.begin(); it != m_renderObjects.end(); ++it)
	{
		auto p = *it;
		SAFE_RELEASE(p);
	}
	m_renderObjects.clear();

	for (auto it = m_lights.begin(); it != m_lights.end(); ++it)
	{
		auto p = *it;
		SAFE_DELETE(p);
	}
	m_lights.clear();

	SAFE_DELETE(m_pMainCamera);
}

void Scene::Update(float dt)
{	
	float angleSpeed = 15;
	for (auto it = m_renderObjects.begin(); it != m_renderObjects.end(); ++it)
	{
		auto pObj = *it;
		if (pObj->enableMove)
			pObj->m_transform.euler.y += angleSpeed * dt;
	}
	CalcBounds();
}

void Scene::CalcBounds()
{
	m_bounds[0].x = m_bounds[0].y = m_bounds[0].z = MAX_FLAT;
	m_bounds[1].x = m_bounds[1].y = m_bounds[1].z = -MAX_FLAT;

	for (auto it = m_renderObjects.begin(); it != m_renderObjects.end(); ++it)
	{
		auto pObj = *it;
		pObj->CalcBounds();
		m_bounds[0] = Vector3::Min(pObj->m_bounds[0], m_bounds[0]);
		m_bounds[1] = Vector3::Max(pObj->m_bounds[1], m_bounds[1]);
	}
}