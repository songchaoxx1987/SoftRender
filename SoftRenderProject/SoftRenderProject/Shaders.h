#pragma once
#include "Vector3.h"
#include "Color.h"
#include <string>
#include <map>

class Vertex;
class Matrix4x4;
class Material;


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

class PSProgramBase
{
public:
	virtual Color Method(Vertex* pVertex, Material* pMat);	
	float AttenShadow(Vertex* pVertex);
};

class PSBlinPhone :public PSProgramBase
{
public:
	virtual Color Method(Vertex* pVertex, Material* pMat);

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

