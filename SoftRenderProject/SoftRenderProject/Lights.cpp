#include "Lights.h"

float Light::Atten(const Vector3& worldPos)
{
	if (mode == LightMode::pointLight)
	{
		float distance = Vector3::Distance(transform.position, worldPos);
		if (distance > pointLightRange)
			return 0;
		return 1.0f - distance / pointLightRange;
	}
	else if (mode == LightMode::directLight)
	{
		return 1.0f;
	}
	return 1.0f;
}