#pragma once
#include "Transform.h"
#include <list>

class CMesh;
class Texture;
class Material;

class RenderObject
{
public:
	RenderObject();
	~RenderObject();
		 
	Transform m_transform;
	CMesh *m_pMesh;
	Material *m_pMaterial;

};

