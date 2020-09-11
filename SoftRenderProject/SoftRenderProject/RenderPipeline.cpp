#include "framework.h"
#include <vector>
#include "RenderPipeline.h"
#include "Matrix4x4.h"
#include "Camera.h"
#include "RenderObject.h"
#include "Vertex.h"
#include "CDevice.h"
#include "CMesh.h"
#include "Trangle.h"
#include "Scene.h"
#include "Material.h"
#include "RenderContext.h"
#include "ShadowMap.h"
#include "Texture.h"
#include "FrameBuffer.h"

#define LERP(a) (pTrangle->v[0].a * lamda1 + pTrangle->v[1].a * lamda2 + pTrangle->v[2].a * lamda3) * z;


void RenderPipeline::Render(Scene* pScene, CDevice* pDevice, Color* pBGColor)
{	
	pScene->m_pMainCamera->BeforeRender();
	RenderContext::pDevice = pDevice;
	RenderContext::m_pLights = &(pScene->m_lights);
	RenderContext::pMainCamera = pScene->m_pMainCamera;
	RenderContext::pAmbColor = &(pScene->ambLight);

	if (RenderContext::pShadowMap == NULL)
		RenderContext::pShadowMap = new ShadowMap();	

	RenderContext::pShadowMap->InitLightCamera(RenderContext::pMainCamera, (*RenderContext::m_pLights)[0], pScene);

	Matrix4x4 v = pScene->m_pMainCamera->GetMatrix_View();
	Matrix4x4 p = pScene->m_pMainCamera->GetMatrix_Proj();
	Matrix4x4 vp = pScene->m_pMainCamera->GetMatrix_VP();
	//Matrix4x4 vp = p * v;
	RenderContext::pView = &v;
	RenderContext::pProj = &p;
	RenderContext::pVP = &vp;	

	RENDER_LIST geometryList;	
	RENDER_LIST alphaList;

	for (RENDER_LIST::iterator it = pScene->m_renderObjects.begin(); it != pScene->m_renderObjects.end(); ++it)
	{
		RenderObject* pObj = *it;
		if (pObj->m_pMaterial->isAlphaBlend || pObj->m_pMaterial->isAlphaTest)
			alphaList.push_back(pObj);
		else
		{
			geometryList.push_back(pObj);			
		}
	}
	
	RenderContext::pMainCamera->GetFrameBuffer()->Clear(pBGColor->r, pBGColor->g, pBGColor->b, pBGColor->a, -MAX_FLAT);
	RenderContext::pShadowMap->GetCamera()->GetFrameBuffer()->Clear(pBGColor->r, pBGColor->g, pBGColor->b, pBGColor->a, -MAX_FLAT);

	RenderAPass(&geometryList, RenderContext::pShadowMap->GetCamera());
	RenderAPass(&geometryList, RenderContext::pMainCamera);
	RenderAPass(&alphaList, RenderContext::pMainCamera);
	
	RenderContext::pMainCamera->GetFrameBuffer()->ApplyToDevice(pDevice);
	pDevice->ApplyToScreen();
}

void RenderPipeline::RenderAPass(RENDER_LIST* pRenderList, Camera* pCamera)
{
	std::vector<Trangle> trangles;
	for (RENDER_LIST::iterator it = pRenderList->begin(); it != pRenderList->end(); ++it)
	{
		trangles.clear();
		RenderObject* pObj = *it;
		Matrix4x4 m2w = pObj->m_transform.Local2World();		
		Matrix4x4 mvp = pCamera->GetMatrix_VP() * m2w;

		RenderContext::pM2W = &m2w;
		RenderContext::pMVP = &mvp;

		auto pFB = pCamera->GetFrameBuffer();

		for (int i = 0; i < pObj->m_pMesh->m_vextexCnt; i += 3)
		{
			Trangle t;
			bool drop = false;
			for (int j = 0; j < 3; ++j)
			{
				// mvp
				t.v[j] = pObj->m_pMesh->m_pVextexs[i + j];
				t.v[j].worldPos = m2w.mul(t.v[j].position);	// worldPos
				//auto v1 = m2w.mul(t.v[j].position);
				//auto v2 = RenderContext::pView->mul(v1);				
				//auto v3 = RenderContext::pProj->mul(v2);
				if (pFB->isFrameBufferAble())
				{					
					pObj->m_pMaterial->ApplyVS(&t.v[j]);
					// 透视除法
					float reciprocalW = 1.0f / t.v[j].position.w;
					t.v[j].position = t.v[j].position * reciprocalW;
					t.v[j].rhw = reciprocalW;
					t.v[j].worldNormal = m2w.mul(t.v[j].normal);
				}
				else
				{	
					t.v[j].position = mvp.mul(t.v[j].position);
					t.v[j].rhw = 1.0f;
				}
				//cvv
				if (!CVVCheck(&t.v[j]))
				{
					drop = true;
					break;
				}
				//视口映射
				t.v[j].position.x = (int)((t.v[j].position.x + 1) * pFB->width() * 0.5f + 0.5f);
				t.v[j].position.y = (int)((1 - t.v[j].position.y) * pFB->height() * 0.5f + 0.5f);
				
			}
			if (!drop)
				trangles.push_back(t);
		}
				
		// 绘制三角形		
		for (int i = 0; i < trangles.size(); ++i)
		{
			auto t = &trangles[i];
			RasterizeATrangle(t, pObj->m_pMaterial, pCamera);
		}		
	}
}

bool RenderPipeline::CVVCheck(Vertex* pVertex)
{
	float w = pVertex->position.w;
	if (pVertex->position.x < -1.0f || pVertex->position.x > 1.0f)
		return false;
	if (pVertex->position.y < -1.0f || pVertex->position.y > 1.0f)
		return false;
	if (pVertex->position.z < -1.0f || pVertex->position.z > 1.0f)
		return false;
	return true;
}



void RenderPipeline::RenderAPassWithCVV(RENDER_LIST* pRenderList, Camera* pCamera)
{
	const Vector3 NDC_PLANES[6] = 
	{
		Vector3(0,0,1,-1),	// n
		Vector3(0,0,-1,-1),	// f
		Vector3(0,1,0,-1),	// t
		Vector3(0,-1,0,-1),	// b
		Vector3(1,0,0,-1),	// r
		Vector3(-1,0,0,-1)	// l
	};
	// 过点p的 平面方程 A*x+B*y+C*z+D=0, 法向量 N = (a,b,c) d = -(n * p) 
	// 对应点 Q ,d = N*Q + D ,d < 0 Q在法线负侧 d > 0 Q在法线正侧
	// 我们约定NDC空间，法线都是沿坐标轴向外的，w存储D值

	auto pFB = pCamera->GetFrameBuffer();
	float w = pFB->width();
	float h = pFB->height();

	std::vector<Trangle> trangles;
	std::vector<Vertex> inputVertexs;
	for (RENDER_LIST::iterator it = pRenderList->begin(); it != pRenderList->end(); ++it)
	{
		trangles.clear();
		RenderObject* pObj = *it;
		Matrix4x4 m2w = pObj->m_transform.Local2World();
		Matrix4x4 mvp = pCamera->GetMatrix_VP() * m2w;
		RenderContext::pM2W = &m2w;
		RenderContext::pMVP = &mvp;

		for (int i = 0; i < pObj->m_pMesh->m_vextexCnt; i += 3)
		{			
			inputVertexs.clear();
			for (int j = 0; j < 3; ++j)
			{
				Vertex v = pObj->m_pMesh->m_pVextexs[i + j];
				if (pFB->isFrameBufferAble())
				{
					v.worldPos = m2w.mul(v.position);	// worldPos			
					v.worldNormal = m2w.mul(v.normal);	// worldNormal

					pObj->m_pMaterial->ApplyVS(&v);
					// 透视除法
					float reciprocalW = 1.0f / v.position.w;
					v.position = v.position * reciprocalW;
					v.rhw = reciprocalW;
				}
				else
				{
					v.position = mvp.mul(v.position);
					v.rhw = 1.0f;
				}
				inputVertexs.push_back(v);
				//视口映射
				v.position.x = (int)((v.position.x + 1) * w * 0.5f + 0.5f);
				v.position.y = (int)((1 - v.position.y) * h * 0.5f + 0.5f);
			}
			if (IsAllOutNDC(inputVertexs))
				continue;

			for (int m = 0; m < 6; ++m)
			{
				for (int n = 0; n < 3; ++n)
				{
					Vertex& s = inputVertexs[n];
					Vertex& e = inputVertexs[(n + 1) % 3];
					if (InsideLine(s.position, NDC_PLANES[i]))
					{
						if (!InsideLine(e.position, NDC_PLANES[i]))
						{
							Vertex intersect = IntersectNDC(s, e, NDC_PLANES[i]);
						}
					}
					else if (InsideLine(e.position, NDC_PLANES[i]))
					{ 
					}
				}
			}

		}

		// 绘制三角形		
		for (int i = 0; i < trangles.size(); ++i)
		{
			auto t = &trangles[i];
			RasterizeATrangle(t, pObj->m_pMaterial, pCamera);
		}
	}
}

bool RenderPipeline::InsideLine(const Vector3& p, const Vector3& line)
{
	return line.x * p.x + line.y * p.y + line.z * p.z + line.w * p.w <= 0;
}

Vertex RenderPipeline::IntersectNDC(const Vertex& s, const Vertex& e, const Vector3& line)
{
	// to do 这里需要判断下点是否超出了线段边界
	float da = s.position.x * line.x + s.position.y * line.y + s.position.z * line.z + line.w * s.position.w;
	float db = e.position.x * line.x + e.position.y * line.y + e.position.z * line.z + line.w * e.position.w;
	float weight = da / (da - db);
	return Vertex::Lerp(s, e, weight);
}

bool RenderPipeline::IsAllOutNDC(const std::vector<Vertex> &v)
{
	for (auto it = v.begin(); it != v.end(); ++it)
	{
		const Vertex* pVertex = &(*it);
		if ((pVertex->position.x >= -1.0f && pVertex->position.x <= 1.0f) && 
			(pVertex->position.y >= -1.0f && pVertex->position.y <= 1.0f) && 
			(pVertex->position.z >= -1.0f && pVertex->position.z <= 1.0f)
			)
			return false;		
	}
	return true;
}

void RenderPipeline::RasterizeATrangle(Trangle* pTrangle, Material* pMat, Camera* pCamera)
{
	// 右手坐标系，逆时针	
	Vector2 ab = pTrangle->v[1].position - pTrangle->v[0].position;
	Vector2 bc = pTrangle->v[2].position - pTrangle->v[1].position;
	Vector2 ca = pTrangle->v[0].position - pTrangle->v[2].position;

	float areaTrangle2 = Vector2::Cross(ab, bc);	// 只处理逆时针的面
	if (areaTrangle2 >= 0)
		return;

	auto pFB = pCamera->GetFrameBuffer();
	int screenHeight = pFB->height();
	int screenWidth = pFB->width();
	
	float pz0 = pTrangle->v[0].position.z;
	float pz1 = pTrangle->v[1].position.z;
	float pz2 = pTrangle->v[2].position.z;

	if (pCamera->Projection() == CAMERA_PROJECTION_MODE::Perspective)
	{
		pz0 = 1.0f / pz0;
		pz1 = 1.0f / pz1;
		pz2 = 1.0f / pz2;
	}

	pTrangle->calcBounds();
	int minX = pTrangle->bounds[0].x;
	int maxX = pTrangle->bounds[1].x;
	int minY = pTrangle->bounds[0].y;
	int maxY = pTrangle->bounds[1].y;

	Vertex v;
	for (int y = minY; y <= maxY && y < screenHeight; ++y)
	{
		bool inside = false;
		for (int x = minX; x <= maxX && x < screenWidth; ++x)
		{
			Vector3 p(x, y, 0);
			Vector2 ap = p - pTrangle->v[0].position;
			Vector2 bp = p - pTrangle->v[1].position;
			Vector2 cp = p - pTrangle->v[2].position;

			float c1 = Vector2::Cross(ab, ap);
			float c2 = Vector2::Cross(bc, bp);
			float c3 = Vector2::Cross(ca, cp);
			if (/*(c1 >= 0 && c2 >= 0 && c3 >= 0) || */(c1 <= 0 && c2 <= 0 && c3 <= 0))
			{
				inside = true;
				float lamda1 = (c2 / areaTrangle2) * pz0;
				float lamda2 = (c3 / areaTrangle2) * pz1;
				float lamda3 = (c1 / areaTrangle2) * pz2;

				float rhw = lamda1 + lamda2 + lamda3;
				float z = rhw;
				if (pCamera->Projection() == CAMERA_PROJECTION_MODE::Perspective)
					z = 1.0 / z;
				if (pFB->isDepthAble())
				{
					if (pMat->zTest && !pFB->ZTest(x,y,z))
						continue;
					if (pMat->zWrite)
						pFB->ZWrite(x, y, z);
				}
				
				if (pFB->isFrameBufferAble())				
				{					
					v.position = p;
					v.position.z = z;
					v.uv = LERP(uv);
					v.normal = LERP(normal);
					v.normal.Normalize();
					v.worldPos = LERP(worldPos);
					v.worldPos.w = 1.0f;
					v.worldNormal = LERP(worldNormal);
					v.worldNormal.Normalize();
					Color ret = pMat->ApplyPS(&v);
					if (pMat->isAlphaTest && pMat->alphaClip > ret.a)
						continue;
					if (pMat->isAlphaBlend)					
						AlphaBlendHandler::AlphaBlendFunction(ret, pFB->GetFrameBuffTex()->textureData[x][y], pMat->srcOp, pMat->destOp, ret);
					pFB->GetFrameBuffTex()->textureData[x][y] = ret;
				}				
			}
			else
			{
				if (inside)
					break;
			}
		}
	}
}
