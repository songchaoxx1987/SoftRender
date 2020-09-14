#include "Trangle.h"

void Trangle::calcBounds()
{
	bounds[0].x = MAX_FLAT;
	bounds[0].y = MAX_FLAT;

	bounds[1].x = -MAX_FLAT;
	bounds[1].y = -MAX_FLAT;

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
