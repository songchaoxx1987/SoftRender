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

#define LERP(a) (pTrangle->v[0].a * lamda1 + pTrangle->v[1].a * lamda2 + pTrangle->v[2].a * lamda3) * z;


void RenderPipeline::Render(Scene* pScene, CDevice* pDevice, UINT32 bgColor)
{
	pDevice->Clear(bgColor);
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
	Matrix4x4 vp = p * v;
	RenderContext::pView = &v;
	RenderContext::pProj = &p;
	RenderContext::pVP = &vp;

	RENDER_LIST geometryList;
	RENDER_LIST alphaList;

	for (RENDER_LIST::iterator it = pScene->m_renderObjects.begin(); it != pScene->m_renderObjects.end(); ++it)
	{
		RenderObject* pObj = *it;
		if (pObj->m_pMaterial->isAlphaBlend || pObj->m_pMaterial->isAplhaTest)
			alphaList.push_back(pObj);
		else
			geometryList.push_back(pObj);
	}
	RenderShadowPass(&geometryList, RenderContext::pShadowMap);
	RenderPass(&geometryList);
	RenderPass(&alphaList);
	pDevice->ApplyToScreen();
}

void RenderPipeline::RenderPass(RENDER_LIST* pRenderList)
{
	std::vector<Trangle> trangles;	
	for (RENDER_LIST::iterator it = pRenderList->begin(); it != pRenderList->end(); ++it)
	{
		trangles.clear();		
		RenderObject* pObj = *it;
		Matrix4x4 m2w = pObj->m_transform.Local2World();
		Matrix4x4 mvp = (*RenderContext::pVP) * m2w;

		RenderContext::pM2W = &m2w;
		RenderContext::pMVP = &mvp;

		for (int i = 0; i < pObj->m_pMesh->m_vextexCnt; i += 3)
		{
			Trangle t;
			bool drop = false;
			for (int j = 0; j < 3; ++j)
			{
				// mvp
				t.v[j] = pObj->m_pMesh->m_pVextexs[i + j];			
				t.v[j].worldPos = RenderContext::pM2W->mul(t.v[j].position);	// worldPos
				pObj->m_pMaterial->ApplyVS(&t.v[j]);				
				// 透视除法
				float reciprocalW = 1.0f / t.v[j].position.w;
				t.v[j].position = t.v[j].position * reciprocalW;
				//cvv
				if (!CVVCheck(&t.v[j]))
				{
					drop = true;
					break;
				}
				//视口映射
				t.v[j].position.x = (int)((t.v[j].position.x + 1) * RenderContext::pDevice->screenWidth * 0.5f + 0.5f);
				t.v[j].position.y = (int)((1 - t.v[j].position.y) * RenderContext::pDevice->screenHeight * 0.5f + 0.5f);
				t.v[j].rhw = reciprocalW;
			}
			if (!drop)
				trangles.push_back(t);
		}

		// 绘制三角形		
		for (int i = 0; i < trangles.size(); ++i)
		{
			auto t = &trangles[i];			
			RasterizeTrangle(t, pObj->m_pMaterial, RenderContext::pDevice->screenWidth, RenderContext::pDevice->screenHeight, RenderMode::forward);
		}
		//printf("pixelCount: %d pixelRealCnt:%d per: %f\n", pDevice->pixelCnt, pDevice->pixelRealCnt,(float)pDevice->pixelRealCnt /(float)pDevice->pixelCnt);
	}
}

void RenderPipeline::RenderShadowPass(RENDER_LIST* pRenderList, ShadowMap* shadowMap)
{
	Camera* pLightCamera = shadowMap->GetCamera();
	Matrix4x4 view = pLightCamera->GetMatrix_View();
	Matrix4x4 proj = pLightCamera->GetMatrix_Proj();
	shadowMap->m_lightVp = proj * view;
	
	std::vector<Trangle> trangles;
	int w = shadowMap->GetTexture()->width;
	int h = shadowMap->GetTexture()->height;
	shadowMap->GetTexture()->ClearTexture(&Color(-MAX_FLAT, 0, 0));
	for (RENDER_LIST::iterator it = pRenderList->begin(); it != pRenderList->end(); ++it)
	{
		trangles.clear();
		RenderObject* pObj = *it;
		Matrix4x4 m2w = pObj->m_transform.Local2World();
		Matrix4x4 mvp = shadowMap->m_lightVp * m2w;
		for (int i = 0; i < pObj->m_pMesh->m_vextexCnt; i += 3)
		{
			Trangle t;
			bool drop = false;
			for (int j = 0; j < 3; ++j)
			{
				// mvp
				t.v[j] = pObj->m_pMesh->m_pVextexs[i + j];
				auto v1 = m2w.mul(t.v[j].position);
				auto v2 = view.mul(v1);
				auto v3 = proj.mul(v2);
				
				t.v[j].position = mvp.mul(t.v[j].position);
				//cvv
				if (!CVVCheck(&t.v[j]))
				{
					drop = true;
					break;
				}
				//视口映射
				t.v[j].position.x = (int)((t.v[j].position.x + 1) * w * 0.5f + 0.5f);
				t.v[j].position.y = (int)((1 - t.v[j].position.y) * h * 0.5f + 0.5f);
				t.v[j].rhw = 1.0f;
			}
			if (!drop)
				trangles.push_back(t);
		}

		// 绘制三角形		
		for (int i = 0; i < trangles.size(); ++i)
		{
			auto t = &trangles[i];
			RasterizeTrangle(t, pObj->m_pMaterial, w, h, RenderMode::shadowmap);
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

void RenderPipeline::RasterizeTrangle(Trangle* pTrangle, Material* pMat, int w, int h, RenderMode mode)
{
	// 右手坐标系，逆时针	
	Vector2 ab = pTrangle->v[1].position - pTrangle->v[0].position;
	Vector2 bc = pTrangle->v[2].position - pTrangle->v[1].position;
	Vector2 ca = pTrangle->v[0].position - pTrangle->v[2].position;

	float areaTrangle2 = Vector2::Cross(ab, bc);	// 只处理逆时针的面
	if (areaTrangle2 >= 0)
		return;
	//areaTrangle2 = abs(areaTrangle2);
	pTrangle->calcBounds();
	int minX = pTrangle->bounds[0].x;
	int maxX = pTrangle->bounds[1].x;
	int minY = pTrangle->bounds[0].y;
	int maxY = pTrangle->bounds[1].y;

	float oneDivZ0 = 1.0f / pTrangle->v[0].position.z;
	float oneDivZ1 = 1.0f / pTrangle->v[1].position.z;
	float oneDivZ2 = 1.0f / pTrangle->v[2].position.z;

	int screenHeight = h;
	int screenWidth = w;
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

				float lamda1 = (c2 / areaTrangle2) * oneDivZ0;
				float lamda2 = (c3 / areaTrangle2) * oneDivZ1;
				float lamda3 = (c1 / areaTrangle2) * oneDivZ2;

				//float lamda1 = abs(c2 / areaTrangle2) * pTrangle->v[0].rhw;
				//float lamda2 = abs(c3 / areaTrangle2) * pTrangle->v[1].rhw;
				//float lamda3 = abs(c1 / areaTrangle2) * pTrangle->v[2].rhw;

				float rhw = lamda1 + lamda2 + lamda3;
				float z = 1.0f / rhw;

				if (mode == RenderMode::forward)
				{
					if (pMat->zTest && !RenderContext::pDevice->ZTest(x, y, z))
						continue;
					if (pMat->zWrite)
						RenderContext::pDevice->ZWrite(x, y, z);
					v.position = p;
					v.position.z = z;
					v.uv = LERP(uv);
					v.normal = LERP(normal);
					v.worldPos = LERP(worldPos);
					//v.uv = (pTrangle->v[0].uv * lamda1 + pTrangle->v[1].uv * lamda2 + pTrangle->v[2].uv * lamda3) * z;
					Draw(&v, pMat);
				}
				else if (mode == RenderMode::deferred)
				{ }
				else if (mode == RenderMode::shadowmap)
				{
					auto shadowTex = RenderContext::pShadowMap->GetTexture();
					if (shadowTex->textureData[x][y].r > z)
						continue;
					shadowTex->textureData[x][y].r = z;
				}
				else
					return;
			}
			else
			{
				if (inside)
					break;
			}
		}
	}
}





//void ForWardRenderPipeline::Render(Scene* pScene, CDevice* pDevice, UINT32 bgColor)
//{	
//	//pDevice->ClearZBuffer();
//	pDevice->Clear(bgColor);
//	pScene->m_pMainCamera->BeforeRender();
//
//	RenderContext::m_pLights = &(pScene->m_lights);
//	RenderContext::pMainCamera = pScene->m_pMainCamera;
//	RenderContext::pAmbColor = &(pScene->ambLight);
//	
//
//	Matrix4x4 v = pScene->m_pMainCamera->GetMatrix_View();
//	Matrix4x4 p = pScene->m_pMainCamera->GetMatrix_Proj();
//	Matrix4x4 vp = p*v ;// pScene->m_pMainCamera->GetMatrix_Proj()* pScene->m_pMainCamera->GetMatrix_View();
//
//	RenderContext::pView = &v;
//	RenderContext::pProj = &p;
//	RenderContext::pVP = &vp;
//
//	std::vector<Trangle> trangles;
//	//std::vector<int> outIdx;
//	RENDER_LIST::iterator it = pScene->m_renderObjects.begin();
//	for (; it != pScene->m_renderObjects.end();++it)
//	{
//		trangles.clear();
//		//outIdx.clear();
//		RenderObject *pObj = *it;
//		Matrix4x4 m2w = pObj->m_transform.Local2World();		
//		Matrix4x4 mvp = vp * m2w;
//
//		RenderContext::pM2W = &m2w;
//		RenderContext::pMVP = &mvp;
//
//		for (int i = 0; i < pObj->m_pMesh->m_vextexCnt; i+=3) 
//		{
//			Trangle t;
//			bool drop = false;
//			for (int j = 0; j < 3; ++j)
//			{
//				// mvp
//				t.v[j] = pObj->m_pMesh->m_pVextexs[i+j];
//				//auto v1 = m2w.mul(t.v[j].position);
//				//auto v2 = v.mul(v1);
//				//auto v3 = p.mul(v2);
//				//auto v4 = v3 * (1.0f / v3.w);
//
//				pObj->m_pMaterial->ApplyVS(&t.v[j]);
//				//t.v[j].position = mvp.mul(t.v[j].position);
//
//				// 透视除法
//				float reciprocalW = 1.0f / t.v[j].position.w;
//				t.v[j].position = t.v[j].position * reciprocalW;
//				//cvv
//				if (!CVVCheck(&t.v[j]))
//				{
//					drop = true;
//					break;
//				}						
//				//视口映射
//				t.v[j].position.x = (int)((t.v[j].position.x + 1) * pDevice->screenWidth * 0.5f + 0.5f);
//				t.v[j].position.y = (int)((1 - t.v[j].position.y) * pDevice->screenHeight * 0.5f + 0.5f);
//				t.v[j].rhw = reciprocalW;
//			}	
//			if (!drop)
//				trangles.push_back(t);			
//		}
//
//
//		// 绘制三角形
//		pDevice->pixelCnt = 0;
//		pDevice->pixelRealCnt = 0;
//		for (int i = 0; i < trangles.size(); ++i)
//		{
//			auto t = &trangles[i];
//			pDevice->RasterizeTrangle(t, pObj->m_pMaterial, this);
//		}
//		//printf("pixelCount: %d pixelRealCnt:%d per: %f\n", pDevice->pixelCnt, pDevice->pixelRealCnt,(float)pDevice->pixelRealCnt /(float)pDevice->pixelCnt);
//	}
//	pDevice->ApplyToScreen();
//}



void ForWardRenderPipeline::Draw(Vertex* v, Material* pMat)
{
	UINT32 rgb = pMat->ApplyPS(v).ToRGB();
	RenderContext::pDevice->DrawPiexl(v->position.x, v->position.y, rgb);
}