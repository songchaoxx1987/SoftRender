#pragma once
#include "define.h"
#include <list>
#include "Vertex.h"

class Camera;
class CDevice;
class RenderObject;

typedef std::list<RenderObject*> RENDER_LIST;


class RenderPipeline
{
public:
	RenderPipeline();
	~RenderPipeline();	
	void Render(RENDER_LIST *pRenderlist, Camera *pCamera, CDevice* pDevice, UINT32 bgColor);
private:
	bool CVVCheck(Vertex *pVertex);
};



