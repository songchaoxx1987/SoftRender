#pragma once
class Color;
class Texture
{
public:
	Color** textureData;
	int width;
	int height;
	bool LoadTexture(const char* Path);
	Color Sample(float u, float v);



};

