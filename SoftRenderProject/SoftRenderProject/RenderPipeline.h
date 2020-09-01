#pragma once
#include "define.h"
#include "Vertex.h"


class CDevice;
class Scene;
class ShadowMap;
class Material;
class RenderObject;
class Trangle;

enum RenderMode
{
	forward = 0,
	deferred,
	shadowmap,
};

class RenderPipeline
{
public:
	void virtual Render(Scene* pScene, CDevice* pDevice, UINT32 bgColor);
	void virtual RasterizeTrangle(Trangle* pTrangle, Material* pMat, int w, int h, RenderMode mode);
protected:
	void RenderPass(RENDER_LIST* pRenderList);
	void virtual Draw(Vertex* pVerTex, Material* pMat) = 0;
	bool CVVCheck(Vertex* pVertex);
	void RenderShadowPass(RENDER_LIST* pRenderList, ShadowMap* shadowMap);
};

class ForWardRenderPipeline :public RenderPipeline
{
public:
	//void virtual Render(Scene* pScene, CDevice* pDevice, UINT32 bgColor);
protected:
	void virtual Draw(Vertex* pVerTex, Material* pMat);

	
};



