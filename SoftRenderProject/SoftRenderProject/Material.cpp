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
			isAlphaTest = atoi(temp[1].c_str()) > 0;
		}
		else if (key == "alphaClip")
		{
			alphaClip = atof(temp[1].c_str());
		}
		else if (key == "alphaBlend")
		{	
			isAlphaBlend = atoi(temp[1].c_str()) > 0;
		}
		else if (key == "alphaBlendOp")
		{
			temp2.clear();
			split(temp[1], temp2, ',');
			srcOp = (AlphaBlendOp)atoi(temp2[0].c_str());
			destOp = (AlphaBlendOp)atoi(temp2[1].c_str());
		}
		else if (key == "zWrite")
		{
			zWrite = atoi(temp[1].c_str()) > 0;
		}
		else if (key == "zTest")
		{
			zTest = atoi(temp[1].c_str()) > 0;
		}
		else if (key == "reciveShadow")
		{
			reciveShadow = atoi(temp[1].c_str()) > 0;
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
	isAlphaTest = false;
	isAlphaBlend = false;
	zWrite = true;
	zTest = true;
	pShader = NULL;
	reciveShadow = true;
}

Material::~Material()
{	
	pTexture = NULL;
	pShader = NULL;
}

Color Material::GetTexColor(float u, float v)
{
	if (!pTexture)
		return Color(1.0f, 1.0f, 1.0f);
	return pTexture->Sample(u, v);
}

Color Material::ApplyPS(Vertex* pVex, RENDER_PATH renderPath)
{
	if (!pShader || !pShader->pPSProgram)
		return GetTexColor(pVex->uv.x, pVex->uv.y) * color;
	Color ret;
	if (renderPath == RENDER_PATH::forward)
	{
		ret = pShader->pPSProgram->ForwardBasePass(pVex, this);
#ifdef ENABLE_ADDPASS
		ret = pShader->pPSProgram->ForwardAddPass(pVex, this, ret);
#endif // ENABLE_ADDPASS
	}
	else if (renderPath == RENDER_PATH::defferd)
	{
		ret = pShader->pPSProgram->DefferdPass(pVex, this);
	}	
	float shadow = 1.0f;
	if (reciveShadow)
		shadow = pShader->pPSProgram->AttenShadow(pVex);
	return ret * shadow;
}

Vertex* Material::ApplyVS(Vertex* pVex)
{
	if (!pShader || !pShader->pVSProgram)			
		return pVex;	
	return pShader->pVSProgram->Method(pVex);
}

void AlphaBlendHandler::AlphaBlendFunction(Color& src, Color& dest, AlphaBlendOp srcOp, AlphaBlendOp destOp, Color& ret)
{
	float p1 = GetAlphaBlendParam(src, dest, srcOp);
	float p2 = GetAlphaBlendParam(src, dest, destOp);
	ret = src * p1 + dest * p2;
}

float AlphaBlendHandler::GetAlphaBlendParam(Color& src, Color& dest, AlphaBlendOp Op)
{
	float p1 = 0;
	switch (Op)
	{
	case AlphaBlendOp::zero:
		p1 = 0;
		break;
	case AlphaBlendOp::one:
		p1 = 1;
		break;
	case AlphaBlendOp::src:
		p1 = src.a;
		break;
	case AlphaBlendOp::dest:
		p1 = dest.a;
		break;
	case AlphaBlendOp::oneMinusSrc:
		p1 = 1 - src.a;
		break;
	case AlphaBlendOp::oneMinusDest:
		p1 = 1 - dest.a;
		break;
	}
	return p1;
}