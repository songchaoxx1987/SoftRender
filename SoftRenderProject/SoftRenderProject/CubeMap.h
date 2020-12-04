#pragma once
#include "define.h"
#include<string>
#include "ResourcesBase.h"

class Texture;
class Color;
class CubeMap :public ResourcesBase
{
public:	

	static std::string CubeMapTexPostName[(int)CUBE_MAP_IDX::count];	
	Color Sample(float x, float y, float z);
	Texture* m_pTexs[(int)CUBE_MAP_IDX::count];
	std::string name;
};

