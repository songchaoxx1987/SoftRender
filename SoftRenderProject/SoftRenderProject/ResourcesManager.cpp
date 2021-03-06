#include "define.h"
#include "ResourcesManager.h"
#include "CMesh.h"
#include "Texture.h"
#include "CubeMap.h"
#include "Material.h"
#include "Color.h"
#include "Shaders.h"

ResourcesManager* ResourcesManager::_pInstance = NULL;

CMesh* ResourcesManager::GetMesh(std::string file)
{
	MAP_MESH::iterator it = m_mapMesh.find(file);
	if (it != m_mapMesh.end())
	{
		it->second->IncreaseRef();
		return it->second;
	}

	CMesh *pMesh = new CMesh();
	char buffer[512];	
	sprintf_s(buffer, "%s\\mesh\\%s.obj", resRootPath.c_str(), file.c_str());
	pMesh->LoadFromFile(buffer);
	pMesh->file = file;
	m_mapMesh[file] = pMesh;
	pMesh->IncreaseRef();
	return pMesh;
}

Texture* ResourcesManager::GetTexture(std::string file)
{
	MAP_TEX::iterator it = m_mapTex.find(file);
	if (it != m_mapTex.end())
	{
		it->second->IncreaseRef();
		return it->second;
	}

	Texture* pTex = new Texture();
	char buffer[512];
	//sprintf_s(buffer, "%s\\tex\\%s.bmp", resRootPath.c_str(), file.c_str());
	sprintf_s(buffer, "%s\\tex\\%s", resRootPath.c_str(), file.c_str());
	pTex->LoadTexture(buffer);
	pTex->file = file;
	m_mapTex[file] = pTex;
	pTex->IncreaseRef();	
	return pTex;
}

CubeMap* ResourcesManager::GetCubeMap(std::string cubeName)
{
	CUBE_MAP::iterator it = m_cubeMap.find(cubeName);
	if (it != m_cubeMap.end())
	{
		it->second->IncreaseRef();
		return it->second;
	}
	CubeMap* pCubeMap = new CubeMap();	
	char buffer[512];
	for (int i = 0 ; i < (int)CUBE_MAP_IDX::count ; ++i)
	{ 
		sprintf_s(buffer, "cubemap\\%s%s.png",cubeName.c_str(), CubeMap::CubeMapTexPostName[i].c_str());
		Texture* pTex = GetTexture(buffer);		
		pCubeMap->m_pTexs[i] = pTex;
	}
	pCubeMap->name = cubeName;
	m_cubeMap[cubeName] = pCubeMap;
	pCubeMap->IncreaseRef();
	return pCubeMap;
}

Material* ResourcesManager::CreateMaterial(std::string matName)
{
	MaterialConfig* pCfg = NULL;
	MAP_MAT_CFG::iterator it = m_mapMatCfg.find(matName);
	if (it == m_mapMatCfg.end())
	{
		char buffer[512];
		sprintf_s(buffer, "%s\\material\\%s.mat", resRootPath.c_str(), matName.c_str());
		pCfg = new MaterialConfig();
		pCfg->LoadFromFile(buffer);
		m_mapMatCfg[matName] = pCfg;		
	}
	else
		pCfg = it->second;

	Material* pMat = new Material();
	if (!pCfg->texFile.empty())
		pMat->pTexture = GetTexture(pCfg->texFile);
	if (!pCfg->cubeMapName.empty())
		pMat->pCubeMap = GetCubeMap(pCfg->cubeMapName);
	pMat->color = pCfg->color;
	pMat->isAlphaBlend = pCfg->isAlphaBlend;
	pMat->isAlphaTest = pCfg->isAlphaTest;
	pMat->zWrite = pCfg->zWrite;
	pMat->zTest = pCfg->zTest;
	pMat->reciveShadow = pCfg->reciveShadow;
	pMat->alphaClip = pCfg->alphaClip;
	pMat->cullOp = pCfg->cullOp;
	pMat->srcOp = pCfg->srcOp;
	pMat->destOp = pCfg->destOp;	
	pMat->pShader = ShaderLib::Instance()->GetShader(pCfg->vsProgram, pCfg->psProgram);
	return pMat;
}

void ResourcesManager::ReleaseMesh(CMesh* pMesh)
{
	if (!pMesh)
		return;
	if (pMesh->DecreaseRef() > 0)
		return;
	MAP_MESH::iterator it = m_mapMesh.find(pMesh->file);
	if (it != m_mapMesh.end())
		m_mapMesh.erase(it);	
	SAFE_RELEASE(pMesh)
}

void ResourcesManager::ReleaseTexture(Texture* pTex)
{
	if (!pTex)
		return;
	if (pTex->DecreaseRef() > 0)
		return;
	MAP_TEX::iterator it = m_mapTex.find(pTex->file);
	if (it != m_mapTex.end())
		m_mapTex.erase(it);
	SAFE_RELEASE(pTex)
}

void ResourcesManager::ReleaseMaterial(Material* pMat)
{
	if (!pMat)
		return;
	if (pMat->pTexture != NULL)
	{
		ReleaseTexture(pMat->pTexture);
		pMat->pTexture == NULL;
	}	
	if (pMat->pCubeMap != NULL)
	{
		ReleaseCubeMap(pMat->pCubeMap);
		pMat->pCubeMap == NULL;
	}
	SAFE_DELETE(pMat)
}


void ResourcesManager::ReleaseCubeMap(CubeMap* pCubeMap)
{
	if (!pCubeMap)
		return;
	if (pCubeMap->DecreaseRef() > 0)
		return;
	for (int i = 0; i < (int)CUBE_MAP_IDX::count; ++i)
	{
		if (pCubeMap->m_pTexs[i] != NULL)
			ReleaseTexture(pCubeMap->m_pTexs[i]);
		pCubeMap->m_pTexs[i] = NULL;
	}
	CUBE_MAP::iterator it = m_cubeMap.find(pCubeMap->name);
	if (it != m_cubeMap.end())
		m_cubeMap.erase(it);
	SAFE_DELETE(pCubeMap)
}