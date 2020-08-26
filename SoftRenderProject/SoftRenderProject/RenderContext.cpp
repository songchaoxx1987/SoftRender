#include "framework.h"
#include "RenderContext.h"

std::vector<Light*>*  RenderContext::m_pLights = NULL;
Camera* RenderContext::pMainCamera = NULL;
Matrix4x4* RenderContext::pM2W = NULL;
Matrix4x4* RenderContext::pView = NULL;
Matrix4x4* RenderContext::pProj = NULL;
Matrix4x4* RenderContext::pVP = NULL;
Matrix4x4* RenderContext::pMVP = NULL;