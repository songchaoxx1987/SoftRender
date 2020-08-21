#pragma once
#include <string>
#include "ResourcesBase.h"
class Color;
class Texture :public ResourcesBase
{
public:
	std::string file;
	Color** textureData;
	int width;
	int height;
	bool LoadTexture(const char* Path);
	Color Sample(float u, float v);

	void Release();

};

