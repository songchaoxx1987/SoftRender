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
	void Render(Scene* pScene, CDevice* pDevice, Color* pBGColor);
protected:
	void RenderAPass(RENDER_LIST* pRenderList, Camera* pCamera);
	void RenderAPassWithCVV(RENDER_LIST* pRenderList, Camera* pCamera);
	void RasterizeATrangle(Trangle* pTrangle, Material* pMat, Camera* pCamera);
	bool CVVCheck(Vertex* pVertex);
	bool IsAllOutNDC(const std::vector<Vertex>& v);	
	bool InsideLine(const Vector3& s, const Vector3& line);
	Vertex IntersectNDC(const Vertex& s, const Vertex& e, const Vector3& line);
};





