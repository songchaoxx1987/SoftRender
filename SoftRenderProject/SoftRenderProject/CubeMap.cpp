#include"framework.h"
#include "CubeMap.h"
#include "Color.h"
#include "Texture.h"


std::string CubeMap::CubeMapTexPostName[6]
{
	"_front",
	"_back",
	"_right",
	"_left",
	"_top",
	"_bottom",
};

Color CubeMap::Sample(float x, float y, float z)
{
	float x1 = abs(x);
	float y1 = abs(y);
	float z1 = abs(z);
	float max = max(x1, max(y1, z1));

	CUBE_MAP_IDX idx;
	float u, v = 0;
	if (max == x1)
	{
		u = z / x1;
		v = y / x1;
		if (x > 0)
		{
			idx = CUBE_MAP_IDX::right;
			u = (u + 1.0) / 2.0;
		}
		else
		{
			idx = CUBE_MAP_IDX::left;
			u = 1.0 - (u + 1.0) / 2.0;
		}
		v = (v + 1.0) / 2.0;
	}
	else if (max == y1)
	{
		u = x / y1;
		v = z / y1;
		if (y > 0)
		{
			idx = CUBE_MAP_IDX::top;
			u = (-u + 1.0) / 2.0;
			v = (-v + 1.0) / 2.0;
		}
		else
		{
			idx = CUBE_MAP_IDX::bottom;
			u = (u + 1.0) / 2.0;
			v = (v + 1.0) / 2.0;
		}
	}
	else if (max == z1)
	{
		u = x / z1;
		v = y / z1;
		if (z > 0)
		{
			idx = CUBE_MAP_IDX::front;
			u = (-u + 1.0) / 2.0;
			v = (-v + 1.0) / 2.0;
		}
		else
		{
			idx = CUBE_MAP_IDX::back;
			u = (u + 1.0) / 2.0;
			v = (v + 1.0) / 2.0;
		}
	}
	return m_pTexs[(int)idx]->Sample(u, v);
}