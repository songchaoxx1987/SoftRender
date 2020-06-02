#include"framework.h"
#include "Material.h"


Material::Material()
{
	pTexture = NULL;
	alpha = 1;
	isAplhaTest = false;
	isAlphaBlend = false;
	zWrite = true;
	zTestOp = 0;
}