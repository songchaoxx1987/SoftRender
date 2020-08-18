#include "Trangle.h"

void Trangle::calcBounds()
{
	bounds[0].x = 9999999;
	bounds[0].y = 9999999;

	bounds[1].x = -999999;
	bounds[1].y = -999999;

	for (int i = 0; i < 3; ++i)
	{
		if (v[i].position.x <= bounds[0].x)		
			bounds[0].x = v[i].position.x;
		if (v[i].position.y <= bounds[0].y)
			bounds[0].y = v[i].position.y;

		if (v[i].position.x >= bounds[1].x)
			bounds[1].x = v[i].position.x;
		if (v[i].position.y >= bounds[1].y)
			bounds[1].y = v[i].position.y;
	}
}