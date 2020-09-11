#include "Lights.h"

static float light_atten0 = 1.0f;
static float light_atten1 = 0.3f;
static float light_atten2 = 0.5f;
float Light::Atten(const Vector3& worldPos)
{
	float p = 1.0f;
	if (mode == LightMode::pointLight)
	{
		float d = Vector3::Distance(transform.position, worldPos);		
		if (d > pointLightRange)
			return 0;
		p = 1.0f / (light_atten0 + light_atten1 * d + light_atten2 * d * d);
	}	
	return p * intensity;
}