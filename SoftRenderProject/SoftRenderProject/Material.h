#pragma once

class Texture;
class Material
{
	Material();	
	Texture* pTexture;
	float alpha;
	bool isAlphaBlend;
	bool isAplhaTest;
	bool zWrite;
	int zTestOp;
};

