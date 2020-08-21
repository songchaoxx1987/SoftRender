#include"framework.h"
#include "Material.h"
#include "Texture.h"
#include <iostream>
#include <fstream>
#include <list>
bool MaterialConfig::LoadFromFile(std::string file)
{
	std::ifstream stream;
	stream.open(file);
	std::string line;
	std::vector<std::string> temp;
	std::vector<std::string> temp2;
	while (getline(stream, line))
	{
		temp.clear();
		split(line, temp, ':');
		if (temp.size() == 0)
			continue;
		auto key = temp[0];	
		if (key == "tex")
		{
			texFile = temp[1];
		}
		else if (key == "color")
		{	
			temp2.clear();
			split(temp[1], temp2, ',');
			color.r = atof(temp2[0].c_str());
			color.g = atof(temp2[1].c_str());
			color.b = atof(temp2[2].c_str());
			color.a = atof(temp2[3].c_str());
		}
		else if (key == "alphaTest")
		{			
			isAplhaTest = atoi(temp[1].c_str()) > 0;
		}
		else if (key == "alphaBlend")
		{	
			isAlphaBlend = atoi(temp[1].c_str()) > 0;
		}
		else if (key == "zWrite")
		{
			zWrite = atoi(temp[1].c_str()) > 0;
		}
		else if (key == "zTest")
		{
			zTest = atoi(temp[1].c_str()) > 0;
		}
	}
	stream.close();
	return true;
}


Material::Material()
{
	pTexture = NULL;
	color = Color(1.0, 1.0, 1.0);
	isAplhaTest = false;
	isAlphaBlend = false;
	zWrite = true;
	zTest = true;
}

Material::~Material()
{	
	pTexture = NULL;
}

UINT32 Material::GetColor(float u, float v)
{
	if (!pTexture)
		return Color::ToRGB(1.0f, 1.0f, 1.0f);
	return (color * pTexture->Sample(u, v)).ToRGB();
}
