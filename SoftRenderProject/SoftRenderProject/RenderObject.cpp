#include "framework.h"
#include "RenderObject.h"
#include "define.h"
#include "ResourcesManager.h"
#include "CMesh.h"

RenderObject::RenderObject()
{
	m_pMesh = NULL;
	m_pMaterial = NULL;
}


RenderObject::~RenderObject()
{
	m_pMesh = NULL;
	m_pMaterial = NULL;
}

void RenderObject::Create(std::string meshFile, std::string matFile)
{
	m_pMesh = ResourcesManager::Instance()->GetMesh(meshFile);
	m_pMaterial = ResourcesManager::Instance()->CreateMaterial(matFile);
}

void RenderObject::Release()
{
	ResourcesManager::Instance()->ReleaseMesh(m_pMesh);
	ResourcesManager::Instance()->ReleaseMaterial(m_pMaterial);
	m_pMesh = NULL;
	m_pMaterial = NULL;
}

void RenderObject::CalcBounds()
{
	auto m = m_transform.Local2World();
	m_bounds[0] = m.mul(m_pMesh->bounds[0]);
	m_bounds[1] = m.mul(m_pMesh->bounds[1]);
}