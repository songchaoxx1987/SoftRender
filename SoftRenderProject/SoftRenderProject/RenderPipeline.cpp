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

RenderPipeline::RenderPipeline()
{	
}


RenderPipeline::~RenderPipeline()
{	
}

void RenderPipeline::Render(RENDER_LIST * pRenderlist, Camera *pCamera, CDevice* pDevice, UINT32 bgColor)
{	
	pDevice->ClearZBuffer();
	pDevice->Clear(bgColor);
	
	Matrix4x4 vp = pCamera->GetMatrix_Proj() * pCamera->GetMatrix_View();
	std::vector<Trangle> trangles;
	std::vector<int> outIdx;
	RENDER_LIST::iterator it = pRenderlist->begin();
	for (; it != pRenderlist->end();++it)
	{
		trangles.clear();
		outIdx.clear();
		RenderObject *pObj = *it;
		Matrix4x4 m2w = pObj->m_transform.Local2World();		
		Matrix4x4 mvp = vp * m2w;
		for (int i = 0; i < pObj->m_pMesh->m_vextexCnt; i+=3) 
		{
			Trangle t;
			bool drop = false;
			for (int j = 0; j < 3; ++j)
			{
				// mvp
				t.v[j] = pObj->m_pMesh->m_pVextexs[i];
				t.v[j].position = mvp.mul(t.v[j].position);
				//cvv
				if (!CVVCheck(&t.v[j]))
				{
					drop = true;
					break;
				}
				// 透视除法
				float reciprocalW = 1.0f / t.v[j].position.w;
				t.v[j].position = t.v[j].position * reciprocalW;	
				// to do uv normal插值
				//视口映射
				t.v[j].position.x = (t.v[j].position.x + 1) * pDevice->screenWidth * 0.5f;
				t.v[j].position.y = (1 - t.v[j].position.x) * pDevice->screenHeight * 0.5f;
				t.v[j].rhw = reciprocalW;
			}	
			if (!drop)
				trangles.push_back(t);			
		}


		// 绘制三角形
		for (int i = 0; i < trangles.size(); ++i)
		{
			auto t = &trangles[i];
			pDevice->RasterizeTrangle(t, pObj->m_pMaterial);
		}
	}
}

bool RenderPipeline::CVVCheck(Vertex *pVertex)
{
	float w = pVertex->position.w;
	if (pVertex->position.x < -w || pVertex->position.x > w)
		return false;
	if (pVertex->position.y < -w || pVertex->position.y > w)
		return false;
	if (pVertex->position.z < -w || pVertex->position.z > w)
		return false;
	return true;
}