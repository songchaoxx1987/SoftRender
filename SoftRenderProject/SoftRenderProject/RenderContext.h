#pragma once
#include <vector>
#include "CTimer.h"
class Light;
class Camera;
class Matrix4x4;
struct Color;

class RenderContext
{
public:	
	static std::vector<Light*> *m_pLights;
	static Camera* pMainCamera;
	static Matrix4x4* pM2W;
	static Matrix4x4* pView;
	static Matrix4x4* pProj;
	static Matrix4x4* pVP;
	static Matrix4x4* pMVP;
	static Color* pAmbColor;
	static double Time()
	{
		return CTimer::Instance()->timePass;
	}
};