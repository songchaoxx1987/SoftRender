#pragma once
#include<string>
#include "Color.h"

class Texture;
class Shader;
class Vertex;
class Matrix4x4;
class CubeMap;


enum AlphaBlendOp
{
	zero = 0,
	one = 1,
	src = 2,
	dest = 3,
	oneMinusSrc = 4,
	oneMinusDest = 5,
};

class MaterialConfig
{
public :
	MaterialConfig()
	{
		isAlphaBlend = false;
		isAlphaTest = false;
		zWrite = true;
		zTest = true;
		reciveShadow = true;
		cullOp = CULL_MODE::cull_front;
	}
	std::string texFile;
	std::string cubeMapName;
	std::string vsProgram;
	std::string psProgram;
	Color color;
	bool isAlphaBlend;
	bool isAlphaTest;
	bool zWrite;
	bool zTest;	
	bool reciveShadow;
	float alphaClip;
	AlphaBlendOp srcOp;
	AlphaBlendOp destOp;
	CULL_MODE cullOp;
	bool LoadFromFile(std::string file);	
};

class Material
{
public:
	Material();
	~Material();
	
	Texture* pTexture;
	CubeMap* pCubeMap;
	Color color;
	Shader* pShader;

	bool isAlphaBlend;
	bool isAlphaTest;
	bool zWrite;
	bool zTest;
	bool reciveShadow;
	float alphaClip;
	AlphaBlendOp srcOp;
	AlphaBlendOp destOp;
	CULL_MODE cullOp;

	Color GetTexColor(float u, float v);
	Color ApplyPS(Vertex* pVex, RENDER_PATH renderPath);
	Vertex* ApplyVS(Vertex* pVex);
	bool isSkyBox;
};

class AlphaBlendHandler
{
public:
	static void AlphaBlendFunction(Color& src, Color& dest, AlphaBlendOp srcOp, AlphaBlendOp destOp, Color &ret);
	static float GetAlphaBlendParam(Color& src, Color& dest, AlphaBlendOp Op);
};
