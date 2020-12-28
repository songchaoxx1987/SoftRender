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


void RenderPipeline::Render(Scene* pScene, CDevice* pDevice, Color* pBGColor)
{	
#ifdef ENABLE_DEFFERED_LIGHTTING
	RENDER_PATH renderPath = RENDER_PATH::defferd;
#else
	RENDER_PATH renderPath = RENDER_PATH::forward;	
#endif

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
	

	if (pScene->m_pSkyBox != NULL)
	{
		RENDER_LIST temp;
		temp.push_back(pScene->m_pSkyBox);
		RenderAPass(&temp, RenderContext::pMainCamera, RENDER_PATH::forward);
	}	
	RenderAPass(&geometryList, RenderContext::pShadowMap->GetCamera(), RENDER_PATH::forward);
	RenderAPass(&geometryList, RenderContext::pMainCamera, renderPath);	
	if (RENDER_PATH::defferd == renderPath)
		DefferedLightting(RenderContext::pMainCamera);
	RenderAPass(&alphaList, RenderContext::pMainCamera, RENDER_PATH::forward);
		
	RenderContext::pMainCamera->GetFrameBuffer()->ApplyToDevice(pDevice);
	pDevice->ApplyToScreen();
}

//void RenderPipeline::RenderAPass2(RENDER_LIST* pRenderList, Camera* pCamera)
//{
//	std::vector<Trangle> trangles;
//	for (RENDER_LIST::iterator it = pRenderList->begin(); it != pRenderList->end(); ++it)
//	{
//		trangles.clear();
//		RenderObject* pObj = *it;
//		Matrix4x4 m2w = pObj->m_transform.Local2World();		
//		Matrix4x4 mvp = pCamera->GetMatrix_VP() * m2w;
//
//		RenderContext::pM2W = &m2w;
//		RenderContext::pMVP = &mvp;
//
//		auto pFB = pCamera->GetFrameBuffer();
//
//		for (int i = 0; i < pObj->m_pMesh->m_vextexCnt; i += 3)
//		{
//			Trangle t;
//			bool drop = false;
//			for (int j = 0; j < 3; ++j)
//			{
//				// mvp
//				t.v[j] = pObj->m_pMesh->m_pVextexs[i + j];
//				t.v[j].worldPos = m2w.mul(t.v[j].position);	// worldPos
//				//auto v1 = m2w.mul(t.v[j].position);
//				//auto v2 = RenderContext::pView->mul(v1);				
//				//auto v3 = RenderContext::pProj->mul(v2);
//				if (pFB->isFrameBufferAble())
//				{					
//					pObj->m_pMaterial->ApplyVS(&t.v[j]);
//					// 透视除法
//					float reciprocalW = 1.0f / t.v[j].position.w;
//					t.v[j].position = t.v[j].position * reciprocalW;
//					t.v[j].rhw = reciprocalW;
//					t.v[j].worldNormal = m2w.mul(t.v[j].normal);
//				}
//				else
//				{	
//					t.v[j].position = mvp.mul(t.v[j].position);
//					t.v[j].rhw = 1.0f;
//				}
//				//cvv
//				if (!CVVCheck(&t.v[j]))
//				{
//					drop = true;
//					break;
//				}
//				//视口映射
//				t.v[j].position.x = (int)((t.v[j].position.x + 1) * pFB->width() * 0.5f + 0.5f);
//				t.v[j].position.y = (int)((1 - t.v[j].position.y) * pFB->height() * 0.5f + 0.5f);
//				
//			}
//			if (!drop)
//				trangles.push_back(t);
//		}
//				
//		// 绘制三角形		
//		for (int i = 0; i < trangles.size(); ++i)
//		{
//			auto t = &trangles[i];
//			RasterizeATrangle(t, pObj->m_pMaterial, pCamera);
//		}		
//	}
//}
//





void RenderPipeline::RenderAPass(RENDER_LIST* pRenderList, Camera* pCamera, RENDER_PATH renderPath)
{	
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
			//Trangle t;
			inputVertexs.clear();
			for (int j = 0; j < 3; ++j)
			{
				Vertex v = pObj->m_pMesh->m_pVextexs[i + j];
				if (pFB->isFrameBufferAble())
				{
					v.worldPos = m2w.mul(v.position);	// worldPos			
					v.worldNormal = m2w.mul(v.normal);	// worldNormal
					pObj->m_pMaterial->ApplyVS(&v);
					
					//float reciprocalW = 1.0f / v.position.w;
					//v.position = v.position * reciprocalW;
					//v.rhw = reciprocalW;
				}
				else
				{
					v.position = mvp.mul(v.position);
					v.position.w = 1.0f;
					//v.rhw = 1.0f;
				}				
				//视口映射
				//v.position.x = (int)((v.position.x + 1) * w * 0.5f + 0.5f);
				//v.position.y = (int)((1 - v.position.y) * h * 0.5f + 0.5f);
				inputVertexs.push_back(v);
			}
					
	
			if (!CVVCheck(&inputVertexs[0], pCamera) ||
				!CVVCheck(&inputVertexs[1], pCamera) ||
				!CVVCheck(&inputVertexs[2], pCamera))
			{			
				SutherlandHodgeman(&inputVertexs);			
			}
						
			for (int i = 0; i < inputVertexs.size(); ++i)
			{
				Vertex* p = &inputVertexs[i];
				p->rhw = 1.0f / p->position.w;
				p->position = p->position * p->rhw;
				// 视口映射
				p->position.x = (int)((p->position.x + 1) * w * 0.5f + 0.5f);
				p->position.y = (int)((1 - p->position.y) * h * 0.5f + 0.5f);
			}

			int n = inputVertexs.size() - 3 + 1;
			int vn = inputVertexs.size();
			for (int i = 0; i < n; i++) 
			{				
				int idx0 = 0;
				int idx1 = i + 2;
				int idx2 = i + 1;				
				// cull
				Vector2 ab = inputVertexs[idx1].position - inputVertexs[idx0].position;
				Vector2 bc = inputVertexs[idx2].position - inputVertexs[idx1].position;
				float areaDouble = Vector2::Cross(ab, bc);
				if (areaDouble == 0 ||
					(areaDouble > 0 && pObj->m_pMaterial->cullOp == CULL_MODE::cull_back) ||
					(areaDouble < 0 && pObj->m_pMaterial->cullOp == CULL_MODE::cull_front))
					continue;
				Trangle t;				
				t.v[0] = inputVertexs[idx0];				
				t.v[1] = inputVertexs[idx1];				
				t.v[2] = inputVertexs[idx2];				
				t.areaDouble = areaDouble;
				t.calcBounds();
				trangles.push_back(t);
			}		
		}

		// 绘制三角形		
		for (int i = 0; i < trangles.size(); ++i)
		{
			auto t = &trangles[i];
			RasterizeATrangle(t, pObj->m_pMaterial, pCamera, renderPath);
		}
	}
}

const Vector3 clip_plane[] = 
{
	Vector3(0,0,1,1),
	Vector3(0,0,-1,1),
	Vector3(0,1,0,1),
	Vector3(0,-1,0,1),
	Vector3(1,0,0,1),
	Vector3(-1,0,0,1),
};

bool RenderPipeline::IsInSide(int clipPlane, const Vector3& pt)
{
	// beause w < 0
	return clip_plane[clipPlane].x * pt.x + clip_plane[clipPlane].y * pt.y + clip_plane[clipPlane].z * pt.z + clip_plane[clipPlane].w * pt.w <= 0;
}

Vertex RenderPipeline::Intersect(const Vertex* pV1, const Vertex* pV2, const Vector3& plane)
{
	float da = pV1->position.x * plane.x + pV1->position.y * plane.y + pV1->position.z * plane.z + pV1->position.w * plane.w;
	float db = pV2->position.x * plane.x + pV2->position.y * plane.y + pV2->position.z * plane.z + pV2->position.w * plane.w;

	float weight = da / (da - db);
	return Vertex::Lerp(*pV1, *pV2, weight);
}

bool RenderPipeline::CVVCheck(Vertex* pVertex, Camera *pCamera)
{
	float w = pVertex->position.w;
	if (w >= pCamera->Near() || w <= pCamera->Far())
		return false;
	if (pVertex->position.x > -w || pVertex->position.x < w)
		return false;
	if (pVertex->position.y > -w || pVertex->position.y < w)
		return false;
	if (pVertex->position.z > -w || pVertex->position.z < w)
		return false;
	return true;
}

void RenderPipeline::SutherlandHodgeman(std::vector<Vertex>* pInput)
{	
	for (int i = 0; i < 6; ++i)
	{
		std::vector<Vertex> input(*pInput);
		pInput->clear();
		for (int j = 0; j < input.size(); j++) 
		{
			Vertex current = input[j];
			Vertex last = input[(j + input.size() - 1) % input.size()];
			if (IsInSide(i, current.position)) 
			{
				if (!IsInSide(i, last.position))
				{
					Vertex intersecting = Intersect(&last, &current, clip_plane[i]);
					pInput->push_back(intersecting);
				}
				pInput->push_back(current);
			}
			else if (IsInSide(i, last.position))
			{
				Vertex intersecting = Intersect(&last, &current, clip_plane[i]);
				pInput->push_back(intersecting);
			}			
		}
		/*
		printf("plane %d\t %f,%f,%f,%f \n", i, clip_plane[i].x, clip_plane[i].y, clip_plane[i].z, clip_plane[i].w);
		for (int k = 0; k < pInput->size(); ++k)
		{
			printf("idx: %d\t %f,%f,%f,%f\n", k, (*pInput)[k].position.x, (*pInput)[k].position.y, (*pInput)[k].position.z, (*pInput)[k].position.w);
		}
		printf("------------------------------------\n");
		*/
	}	
}



void RenderPipeline::RasterizeATrangle(Trangle* pTrangle, Material* pMat, Camera* pCamera, RENDER_PATH renderPath)
{
	// 右手坐标系，逆时针	
	Vector2 ab = pTrangle->v[1].position - pTrangle->v[0].position;
	Vector2 bc = pTrangle->v[2].position - pTrangle->v[1].position;
	Vector2 ca = pTrangle->v[0].position - pTrangle->v[2].position;

	float areaTrangle2 = pTrangle->areaDouble;
	//float areaTrangle2 = Vector2::Cross(ab, bc);	// 只处理逆时针的面
	//if (areaTrangle2 >= 0)
	//	return;

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

	//pTrangle->calcBounds();
	int minX = max(pTrangle->bounds[0].x, 0.0f);
	int maxX = min(pTrangle->bounds[1].x, screenWidth);
	int minY = max(pTrangle->bounds[0].y, 0.0f);
	int maxY = min(pTrangle->bounds[1].y, screenHeight);

	Vertex v;
	Vector3 p(0, 0, 0);
	for (int y = minY; y <= maxY && y < screenHeight; ++y)
	{
		bool inside = false;
		for (int x = minX; x <= maxX && x < screenWidth; ++x)
		{
			p.x = x;
			p.y = y;
			p.z = 0;			
			Vector2 ap = p - pTrangle->v[0].position;
			Vector2 bp = p - pTrangle->v[1].position;
			Vector2 cp = p - pTrangle->v[2].position;

			float c1 = Vector2::Cross(ab, ap);
			float c2 = Vector2::Cross(bc, bp);
			float c3 = Vector2::Cross(ca, cp);			
			if ((c1 >= 0 && c2 >= 0 && c3 >= 0 && (areaTrangle2 > 0 || pMat->cullOp == CULL_MODE::cull_off)) ||
				(c1 <= 0 && c2 <= 0 && c3 <= 0 && (areaTrangle2 < 0 || pMat->cullOp == CULL_MODE::cull_off)))
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
					if (pMat->zTest && !pFB->ZTest(x, y, z))
						continue;
					if (pMat->zWrite)
						pFB->ZWrite(x, y, z);									
				}				
				if (pFB->OnlyDep())
					continue;
				if (renderPath == RENDER_PATH::forward)
				{ 
					v.position = p;
					v.position.z = z;
					v.BarycentricLerp(pTrangle->v[0], pTrangle->v[1], pTrangle->v[2], lamda1, lamda2, lamda3);
					Color ret = pMat->ApplyPS(&v, RENDER_PATH::forward);
					if (pMat->isAlphaTest && pMat->alphaClip > ret.a)
						continue;
					if (pMat->isAlphaBlend)
						AlphaBlendHandler::AlphaBlendFunction(ret, pFB->GetFrameBuffTex()->textureData[x][y], pMat->srcOp, pMat->destOp, ret);
					pFB->GetFrameBuffTex()->textureData[x][y] = ret;
				}
				else if (renderPath == RENDER_PATH::defferd)
				{						
					/*if (pMat->isAlphaBlend || pMat->isAlphaTest)
					{
						v.position = p;
						v.position.z = z;
						v.BarycentricLerp(pTrangle->v[0], pTrangle->v[1], pTrangle->v[2], lamda1, lamda2, lamda3);
						Color ret = pMat->ApplyPS(&v, RENDER_PATH::forward);
						if (pMat->isAlphaTest && pMat->alphaClip > ret.a)
							continue;
						if (pMat->isAlphaBlend)
							AlphaBlendHandler::AlphaBlendFunction(ret, pFB->GetFrameBuffTex()->textureData[x][y], pMat->srcOp, pMat->destOp, ret);
						pFB->GetFrameBuffTex()->textureData[x][y] = ret;
					}
					else*/
					{
						auto pFragment = pFB->GetGFragment(x, y);
						pFragment->pVertex->position = p;
						pFragment->pVertex->position.z = z;
						pFragment->pVertex->BarycentricLerp(pTrangle->v[0], pTrangle->v[1], pTrangle->v[2], lamda1, lamda2, lamda3);
						pFragment->pMaterial = pMat;
						pFragment->usedFlag = true;
					}				
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

void RenderPipeline::DefferedLightting(Camera* pCamera)
{
	RenderContext::pMainCamera = pCamera;
	auto pFB = pCamera->GetFrameBuffer();
	for (int x = 0; x < pFB->width(); ++x)
	{
		for (int y = 0; y < pFB->height(); ++y)
		{			
			auto *pFragment = pFB->GetGFragment(x, y);
			if (!pFragment->usedFlag)
				continue;
			Color ret = pFragment->pMaterial->ApplyPS(pFragment->pVertex, RENDER_PATH::defferd);
			pFB->GetFrameBuffTex()->textureData[x][y] = ret;
		}
	}
	
}