#include "framework.h"
#include "RenderObject.h"
#include "define.h"


RenderObject::RenderObject()
{
}


RenderObject::~RenderObject()
{
	m_pMesh = NULL;
	m_pMaterial = NULL;
}
