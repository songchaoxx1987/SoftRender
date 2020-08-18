#include"framework.h"
#include "Material.h"
#include "Texture.h"
#include "Color.h"

Material::Material()
{
	pTexture = NULL;
	pColor = new Color(1.0, 1.0, 1.0);
	isAplhaTest = false;
	isAlphaBlend = false;
	zWrite = true;
	zTest = true;
}

UINT32 Material::GetColor(float u, float v)
{
	if (!pTexture)
		return Color::ToRGB(1.0f, 1.0f, 1.0f);
	return ((*pColor) * pTexture->Sample(u, v)).ToRGB();
}