#pragma once

struct  Color;
class Texture;
class Material
{
public:
	Material();	
	Texture* pTexture;
	Color* pColor;

	bool isAlphaBlend;
	bool isAplhaTest;
	bool zWrite;
	bool zTest;
	//int cullOp;	// 0 ≤ªºÙ≤√ 1 À≥ ±’Î 2 ƒÊ ±’Î 

	UINT32 GetColor(float u, float v);
};

