#pragma once
#include "Transform.h"
#include <list>
#include <string>

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
	bool enableMove = false;
	Vector3 m_bounds[2];

	void Create(std::string meshFile, std::string matFile);
	void Release();
	void CalcBounds();
};

