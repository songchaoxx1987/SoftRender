#pragma once

#include "Transform.h"
#include "Color.h"
#include "define.h"

enum LightMode
{
	directLight = 0,
	pointLight,
};

class Light
{
public:
	Transform transform;
	Color color;
	LightMode mode;
	float pointLightRange;
	bool castShadow;
	void CalcDir()
	{
		if (mode == LightMode::directLight)
		{
			_dir = transform.Local2World().mul(Vector3(0, 0, 1, 0));
			_dir.Normalize();
		}
	}

	Vector3 Dir()
	{
		return _dir;
	}

	Vector3 InvDir()
	{
		return -_dir;
	}
		
	float Atten(const Vector3& worldPos);	
protected:
	Vector3 _dir;
};
