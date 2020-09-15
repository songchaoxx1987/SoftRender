#pragma once
#include<string>
#include "Color.h"

class Texture;
class Shader;
class Vertex;
class Matrix4x4;


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
	}
	std::string texFile;
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
	bool LoadFromFile(std::string file);	
};

class Material
{
public:
	Material();
	~Material();
	
	Texture* pTexture;
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
	//int cullOp;	// 0 ≤ªºÙ≤√ 1 À≥ ±’Î 2 ƒÊ ±’Î 

	Color GetTexColor(float u, float v);
	Color ApplyPS(Vertex* pVex, RENDER_PATH renderPath);
	Vertex* ApplyVS(Vertex* pVex);
};

class AlphaBlendHandler
{
public:
	static void AlphaBlendFunction(Color& src, Color& dest, AlphaBlendOp srcOp, AlphaBlendOp destOp, Color &ret);
	static float GetAlphaBlendParam(Color& src, Color& dest, AlphaBlendOp Op);
};
