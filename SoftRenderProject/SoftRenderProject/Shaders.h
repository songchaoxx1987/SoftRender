#pragma once
#include "Vector3.h"
#include "Color.h"
#include <string>
#include <map>

class Vertex;
class Matrix4x4;
class Material;
class Light;

class VSProgramBase
{
public:
	virtual Vertex* Method(Vertex* pVertex);
};

class VSWave :public VSProgramBase
{
public:
	virtual Vertex* Method(Vertex* pVertex);
};

class VSBlinPhone :public VSProgramBase
{
public:
	virtual Vertex* Method(Vertex* pVertex);
};

class VSSkyBox :public VSProgramBase
{
public:
	virtual Vertex* Method(Vertex* pVertex);
};

class PSProgramBase
{
public:
	virtual Color ForwardBasePass(Vertex* pVertex, Material* pMat);	
	virtual Color ForwardAddPass(Vertex* pVertex, Material* pMat, Color& baseColor) { return baseColor; }
	virtual Color DefferdPass(Vertex* pVertex, Material* pMat);
	float AttenShadow(Vertex* pVertex);	
};

class PSBlinPhone :public PSProgramBase
{
public:
	Color specColor = Color(1.0, 1.0, 1.0);
	float gloss = 1;
	virtual Color ForwardBasePass(Vertex* pVertex, Material* pMat);
	virtual Color ForwardAddPass(Vertex* pVertex, Material* pMat, Color& baseColor);
	virtual Color DefferdPass(Vertex* pVertex, Material* pMat);

	Color LightFunction(Light* pLight, Vertex* pVex, const Color& diffuseColor);
};

class HalfLambertDiffuse :public PSProgramBase
{
public:	
	virtual Color ForwardBasePass(Vertex* pVertex, Material* pMat);
	virtual Color ForwardAddPass(Vertex* pVertex, Material* pMat, Color& baseColor);
	virtual Color DefferdPass(Vertex* pVertex, Material* pMat);

	Color LightFunction(Light* pLight, Vertex* pVex);
};

class PSSkyBox :public PSProgramBase
{
public:
	virtual Color ForwardBasePass(Vertex* pVertex, Material* pMat);
	virtual Color ForwardAddPass(Vertex* pVertex, Material* pMat, Color& baseColor) { return baseColor; }
	virtual Color DefferdPass(Vertex* pVertex, Material* pMat);
	Color GetRet(Vertex* pVertex, Material* pMat);
};

class Shader
{
public:
	VSProgramBase* pVSProgram;
	PSProgramBase* pPSProgram;
};

class ShaderLib
{
	static ShaderLib* _pInstance;
	ShaderLib();

	typedef std::map<std::string, Shader*> Shader_Map;
	Shader_Map m_shadersMap;

	std::map<std::string, PSProgramBase*> m_psMap;
	std::map<std::string, VSProgramBase*> m_vsMap;
public:	
	static ShaderLib* Instance()
	{
		if (!_pInstance)
			_pInstance = new ShaderLib();
		return _pInstance;
	}

	Shader* GetShader(std::string vs, std::string ps);
};

