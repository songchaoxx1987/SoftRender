#pragma once
#include <string>
#include "ResourcesBase.h"
class Color;
class Texture :public ResourcesBase
{
public:
	Texture()
	{
		UV_STARTS_AT_TOP = false;
	}
	std::string file;
	Color** textureData;
	int width;
	int height;

	bool UV_STARTS_AT_TOP;	// true uv坐标00点在左上角

	bool LoadTexture(const char* Path);
	Color Sample(float u, float v);
	void SetPixel(float u, float v, Color &color);

	void CreateTexture(int w, int h, Color *pClr, bool topUv);
	void ClearTexture(Color* pClr);

	void Release();

	void SavePng(std::string file);
};

