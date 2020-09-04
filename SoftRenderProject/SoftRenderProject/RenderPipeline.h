#pragma once
#include "define.h"


class CDevice;
class Scene;
class ShadowMap;
class Material;
class RenderObject;
class Trangle;
class Camera;
class Vertex;
struct Color;

class RenderPipeline
{
public:
	void Render(Scene* pScene, CDevice* pDevice, Color* pBGColor);
protected:
	void RenderAPass(RENDER_LIST* pRenderList, Camera* pCamera);
	void RasterizeATrangle(Trangle* pTrangle, Material* pMat, Camera* pCamera);
	bool CVVCheck(Vertex* pVertex);
};





