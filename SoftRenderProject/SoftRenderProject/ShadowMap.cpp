#include "ShadowMap.h"
#include "define.h"
#include "Camera.h"
#include "Scene.h"
#include "Texture.h"
#include "Vector3.h"

#define SHADOWMAP_SIZE 1024

ShadowMap::ShadowMap()
{	
	_pLightCamera = new Camera();
	_pTexture = new Texture();
	_pTexture->CreateTexture(SHADOWMAP_SIZE, SHADOWMAP_SIZE, NULL);	
}

ShadowMap::~ShadowMap()
{
	SAFE_DELETE(_pLightCamera);
	SAFE_RELEASE(_pTexture);
}

Camera* ShadowMap::InitLightCamera(Camera *pMainCamera, Light* pLight, Scene *pScene)
{	
	pLight->CalcDir();
	_pLightCamera->SetPosition(Vector3(0, 0, 0));
	_pLightCamera->SetLookAt(_pLightCamera->Position() + pLight->Dir() );
	auto viewM = _pLightCamera->GetMatrix_View();

	Vector3* pCornerInWorld = pMainCamera->GetWorldCorner(20);	
	Vector3 cornersInView[8];
	TransformArray(&viewM, pCornerInWorld, cornersInView, 8);
	Vector3 viewBounds[2];
	CalcBounds(cornersInView, viewBounds, 8);

	Vector3 sceneBounds[2];
	TransformArray(&viewM, pScene->m_bounds, sceneBounds, 2);
	Vector3 boundsInView[2];
	CalcBounds(sceneBounds, boundsInView, 2);

	Vector3 camPos;
	camPos.x = (viewBounds[1].x + viewBounds[0].x) / 2.0f;
	camPos.y = (viewBounds[1].y + viewBounds[0].y) / 2.0f;
	camPos.z = boundsInView[1].z;

	auto invLightCamView = _pLightCamera->GetMatrix_InvView();
	camPos = invLightCamView.mul(camPos);
	_pLightCamera->SetPosition(camPos);
	_pLightCamera->SetLookAt(camPos + pLight->Dir());
	float size = abs(viewBounds[1].y - viewBounds[0].y)/2.0f;
	float aspect = abs((viewBounds[1].x - viewBounds[0].x) / (viewBounds[1].y - viewBounds[0].y));
	_pLightCamera->SetOrthoCameraInfo(size, aspect, 0, boundsInView[1].z - boundsInView[0].z);
	
	return _pLightCamera;
}

bool ShadowMap::IsInShadows(Vector3* pWorldPos)
{
	auto pos = m_lightVp.mul(*pWorldPos);
	int u = (int)((pos.x + 1) * _pTexture->width * 0.5f + 0.5f);
	int v = (int)((1 - pos.y) * _pTexture->height * 0.5f + 0.5f);
	return _pTexture->textureData[u][v].r > pos.z;
}