#pragma once
#include "define.h"
#include <list>
#include "Vertex.h"


class CDevice;
class Scene;
class RenderPipeline
{
public:
	RenderPipeline();
	~RenderPipeline();	
	void Render(Scene* pScene, CDevice* pDevice, UINT32 bgColor);
private:
	bool CVVCheck(Vertex *pVertex);
};



