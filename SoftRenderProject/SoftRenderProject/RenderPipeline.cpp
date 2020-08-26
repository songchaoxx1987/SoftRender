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

RenderPipeline::RenderPipeline()
{	
}


RenderPipeline::~RenderPipeline()
{	
}

void RenderPipeline::Render(Scene* pScene, CDevice* pDevice, UINT32 bgColor)
{	
	//pDevice->ClearZBuffer();
	pDevice->Clear(bgColor);
	
	RenderContext::m_pLights = &(pScene->m_lights);
	RenderContext::pMainCamera = pScene->m_pMainCamera;

	Matrix4x4 v = pScene->m_pMainCamera->GetMatrix_View();
	Matrix4x4 p = pScene->m_pMainCamera->GetMatrix_Proj();
	Matrix4x4 vp = p*v ;// pScene->m_pMainCamera->GetMatrix_Proj()* pScene->m_pMainCamera->GetMatrix_View();

	RenderContext::pView = &v;
	RenderContext::pProj = &p;
	RenderContext::pVP = &vp;

	std::vector<Trangle> trangles;
	//std::vector<int> outIdx;
	RENDER_LIST::iterator it = pScene->m_renderObjects.begin();
	for (; it != pScene->m_renderObjects.end();++it)
	{
		trangles.clear();
		//outIdx.clear();
		RenderObject *pObj = *it;
		Matrix4x4 m2w = pObj->m_transform.Local2World();		
		Matrix4x4 mvp = vp * m2w;

		RenderContext::pM2W = &m2w;
		RenderContext::pMVP = &mvp;

		for (int i = 0; i < pObj->m_pMesh->m_vextexCnt; i+=3) 
		{
			Trangle t;
			bool drop = false;
			for (int j = 0; j < 3; ++j)
			{
				// mvp
				t.v[j] = pObj->m_pMesh->m_pVextexs[i+j];
				//auto v1 = m2w.mul(t.v[j].position);
				//auto v2 = v.mul(v1);
				//auto v3 = p.mul(v2);
				//auto v4 = v3 * (1.0f / v3.w);

				pObj->m_pMaterial->ApplyVS(&t.v[j]);
				//t.v[j].position = mvp.mul(t.v[j].position);

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
				t.v[j].position.x = (int)((t.v[j].position.x + 1) * pDevice->screenWidth * 0.5f + 0.5f);
				t.v[j].position.y = (int)((1 - t.v[j].position.y) * pDevice->screenHeight * 0.5f + 0.5f);
				t.v[j].rhw = reciprocalW;
			}	
			if (!drop)
				trangles.push_back(t);			
		}


		// 绘制三角形
		pDevice->pixelCnt = 0;
		pDevice->pixelRealCnt = 0;
		for (int i = 0; i < trangles.size(); ++i)
		{
			auto t = &trangles[i];
			pDevice->RasterizeTrangle(t, pObj->m_pMaterial);
		}
		//printf("pixelCount: %d pixelRealCnt:%d per: %f\n", pDevice->pixelCnt, pDevice->pixelRealCnt,(float)pDevice->pixelRealCnt /(float)pDevice->pixelCnt);
	}
	pDevice->ApplyToScreen();
}

bool RenderPipeline::CVVCheck(Vertex *pVertex)
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