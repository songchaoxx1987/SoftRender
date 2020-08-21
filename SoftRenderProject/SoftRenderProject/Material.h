#pragma once
#include<string>
#include "Color.h"

class Texture;


class MaterialConfig
{
public :
	std::string texFile;
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

	bool isAlphaBlend;
	bool isAplhaTest;
	bool zWrite;
	bool zTest;
	//int cullOp;	// 0 ������ 1 ˳ʱ�� 2 ��ʱ�� 

	UINT32 GetColor(float u, float v);	
};


