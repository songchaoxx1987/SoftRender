#pragma once
#include<string>
#include "Color.h"

class Texture;
class Shader;
class Vertex;
class Matrix4x4;

class MaterialConfig
{
public :
	std::string texFile;
	std::string vsProgram;
	std::string psProgram;
	Color color;
	bool isAlphaBlend;
	bool isAplhaTest;
	bool zWrite;
	bool zTest;
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
	bool isAplhaTest;
	bool zWrite;
	bool zTest;
	//int cullOp;	// 0 ≤ªºÙ≤√ 1 À≥ ±’Î 2 ƒÊ ±’Î 

	Color GetColor(float u, float v);
	Color ApplyPS(Vertex* pVex);
	Vertex* ApplyVS(Vertex* pVex);
};


