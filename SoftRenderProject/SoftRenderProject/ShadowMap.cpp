#include"framework.h"
#include "ShadowMap.h"
#include "define.h"
#include "Camera.h"
#include "Scene.h"
#include "Lights.h"
#include "Texture.h"
#include "Vector3.h"
#include "Vertex.h"
#include "RenderContext.h"
#include "FrameBuffer.h"

#define SHADOWMAP_SIZE 1024
#define DEFAULT_BIAS 0.005f

ShadowMap::ShadowMap()
{	
	_pLightCamera = new Camera();
	int flag = SET_FLAG(0, ENUM_FB_MODE::depth);	
	_pLightCamera->CreateFrameBuffer(SHADOWMAP_SIZE, SHADOWMAP_SIZE, flag);	
}

ShadowMap::~ShadowMap()
{
	SAFE_DELETE(_pLightCamera);	
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
	CalcBounds(cornersInView, viewBounds, 8, 1.0f);

	Vector3 sceneCornersInLightSpace[8];
	TransformArray(&viewM, pScene->m_corners, sceneCornersInLightSpace, 8);
	Vector3 sceneBoundsInLightSpace[2];
	CalcBounds(sceneCornersInLightSpace, sceneBoundsInLightSpace, 8, 1.0f);
	
	Vector3 camPosInView;
	camPosInView.x = (viewBounds[1].x + viewBounds[0].x) / 2.0f;
	camPosInView.y = (viewBounds[1].y + viewBounds[0].y) / 2.0f;	
	camPosInView.z = sceneBoundsInLightSpace[1].z;

	auto invLightCamView = _pLightCamera->GetMatrix_InvView();	
	Vector3 camPos = invLightCamView.mul(camPosInView);
	_pLightCamera->SetPosition(camPos);
	_pLightCamera->SetLookAt(camPos + pLight->Dir());
	float size = abs(viewBounds[1].y - viewBounds[0].y)/2.0f;
	float aspect = abs((viewBounds[1].x - viewBounds[0].x) / (viewBounds[1].y - viewBounds[0].y));
	_pLightCamera->SetOrthoCameraInfo(size, aspect, 0, 1.1f * (sceneBoundsInLightSpace[1].z - sceneBoundsInLightSpace[0].z));
	_pLightCamera->BeforeRender();
	return _pLightCamera;
}

float ShadowMap::AttenShadow(Vertex* pVertex)
{
	Light* pLight = (*RenderContext::m_pLights)[0];
	auto lightDir = pLight->InvDir();
	float dot = Vector3::Dot(pVertex->normal, lightDir);
	if (dot < 0)
		return 1.0f;
	float bias = max(0.05f * (1.0f - dot), DEFAULT_BIAS);	
	auto pos = _pLightCamera->GetMatrix_VP().mul(pVertex->worldPos);

	auto pFB = _pLightCamera->GetFrameBuffer();
	int w = pFB->width();
	int h = pFB->height();
		
	int u = (int)((pos.x + 1) * w * 0.5f + 0.5f);
	int v = (int)((1 - pos.y) * h * 0.5f + 0.5f);
	//float realZ = ShadowMap::zInShadowMap(pos.z) - bias;	
	float realZ = pos.z + bias;

#ifdef  ENABLE_SHADOWMAP_PCF
	int total = 0;
	int inshadows = 0;
	for (int fu = u - 1; fu < u + 1 && fu < w; ++fu)
	{
		for (int fv = v - 1; fv < v + 1 && fv < h; ++fv)
		{
			++total;			
			//if (ShadowMap::zInShadowMap(pFB->depth(fu,fv)) < realZ)
			if (pFB->depth(fu, fv) > realZ)
				++inshadows;
		}
	}	
	return Clamp<float>((1.0 - (float)inshadows / (float)total), 0.2f, 1.0f);
#else	
	//return ShadowMap::zInShadowMap(pFB->depth(u, v)) < realZ ? 0.2f : 1.0f;
	return pFB->depth(u, v) > realZ ? 0.2f : 1.0f;
#endif //  ENABLE_SHADOWMAP_PCF
}

void ShadowMap::SaveShadowMap2PNG(std::string file)
{
	auto pFP = _pLightCamera->GetFrameBuffer();
	Texture tex;
	Color c = Color(1.0f, 1.0f, 1.0f, 1.0f);
	tex.CreateTexture(pFP->width(), pFP->height(), &c, false);
	for (int y = 0; y < pFP->height(); ++y)
	{
		for (int x = 0; x < pFP->width(); ++x)
		{
			float z = zInShadowMap(pFP->depth(x, y));
			tex.textureData[x][y].r = z;
			tex.textureData[x][y].g = z;
			tex.textureData[x][y].b = z;
		}
	}
	tex.SavePng(file);
}

