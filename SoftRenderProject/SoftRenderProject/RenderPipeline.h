#pragma once
#include "define.h"
#include "Vertex.h"

class CDevice;
class Scene;
class ShadowMap;
class Material;
class RenderObject;
class Trangle;
class Camera;
//class Vertex;
struct Color;

class RenderPipeline
{
public:
//	RENDER_PATH m_renderPath;
	void Render(Scene* pScene, CDevice* pDevice, Color* pBGColor);
protected:	
	void RenderAPass(RENDER_LIST* pRenderList, Camera* pCamera, RENDER_PATH renderPath);
	void RasterizeATrangle(Trangle* pTrangle, Material* pMat, Camera* pCamera, RENDER_PATH renderPath);

	void DefferedLightting(Camera* pCamera);
	//void RenderAPass2(RENDER_LIST* pRenderList, Camera* pCamera);
	//bool CVVCheck(Vertex* pVertex);	
};





