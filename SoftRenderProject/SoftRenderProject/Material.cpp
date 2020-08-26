#include"framework.h"
#include "Material.h"
#include "Texture.h"
#include "Vertex.h"
#include "Shaders.h"
#include "Matrix4x4.h"
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
		else if (key == "vs")
		{
			vsProgram = temp[1];
		}
		else if (key == "ps")
		{
			psProgram = temp[1];
		}
	}
	stream.close();
	if (vsProgram.empty())
		vsProgram = "default";
	if (psProgram.empty())
		psProgram = "default";
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
	pShader = NULL;
}

Material::~Material()
{	
	pTexture = NULL;
	pShader = NULL;
}

Color Material::GetColor(float u, float v)
{
	if (!pTexture)
		return Color(1.0f, 1.0f, 1.0f);
	return (color * pTexture->Sample(u, v));
}

Color Material::ApplyPS(Vertex* pVex)
{
	if (!pShader || !pShader->pPSProgram)
		return GetColor(pVex->uv.x, pVex->uv.y);
	return pShader->pPSProgram->Method(pVex, this);
}

Vertex* Material::ApplyVS(Vertex* pVex)
{
	if (!pShader || !pShader->pVSProgram)			
		return pVex;	
	return pShader->pVSProgram->Method(pVex);
}