#pragma once
#include <map>
#include <string>

class CMesh;
class Texture;
class Material;
class MaterialConfig;

class ResourcesManager
{
	static ResourcesManager* _pInstance;
	ResourcesManager() {}
public:
	static ResourcesManager* Instance()
	{
		if (!_pInstance)
			_pInstance = new ResourcesManager();
		return _pInstance;
	}


	CMesh* GetMesh(std::string file);
	Texture* GetTexture(std::string file);
	Material* CreateMaterial(std::string matName);

	void ReleaseMesh(CMesh* pMesh);
	void ReleaseTexture(Texture* pTex);
	void ReleaseMaterial(Material* pMat);

	std::string resRootPath;

private:
	typedef std::map<std::string, CMesh*> MAP_MESH;
	MAP_MESH m_mapMesh;

	typedef std::map<std::string, Texture*> MAP_TEX;
	MAP_TEX m_mapTex;

	typedef std::map<std::string, MaterialConfig*> MAP_MAT_CFG;

	MAP_MAT_CFG m_mapMatCfg;

};

