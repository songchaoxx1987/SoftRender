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
	//int cullOp;	// 0 ������ 1 ˳ʱ�� 2 ��ʱ�� 

	UINT32 GetColor(float u, float v);
};

