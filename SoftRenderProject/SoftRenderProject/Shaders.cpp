#include"framework.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Matrix4x4.h"
#include "Material.h"
#include "CTimer.h"
#include "RenderContext.h"
#include "Scene.h"
#include "Camera.h"

ShaderLib* ShaderLib::_pInstance = NULL;

Vertex* VSProgramBase::Method(Vertex* pVertex)
{
	Matrix4x4* pMvp = RenderContext::pMVP;
	pVertex->position = pMvp->mul(pVertex->position);
	return pVertex;
}

Vertex* VSWave::Method(Vertex* pVertex)
{
	auto pos = pVertex->position;
	pos.y += 0.1f*sin(PI * RenderContext::Time() * pos.x * pos.z);
	pVertex->position = RenderContext::pMVP->mul(pos);
	return pVertex;
}

Color PSProgramBase::Method(Vertex* pVertex, Material* pMat)
{	
	return pMat->GetColor(pVertex->uv.x, pVertex->uv.y);
}

Vertex* VSBlinPhone::Method(Vertex* pVertex)
{
	pVertex->position = RenderContext::pMVP->mul(pVertex->position);
	pVertex->normal = RenderContext::pM2W->mul(pVertex->normal);
	pVertex->uv1 = RenderContext::pM2W->mul(pVertex->position);	// worldPos
	return pVertex;
}

Color PSBlinPhone::Method(Vertex* pVertex, Material* pMat)
{
	pVertex->normal.Normalize();
	Light* pLight = (*RenderContext::m_pLights)[0];
	auto lightDir = -pLight->Dir();

	Color diffuse = pLight->color * max(0, Vector3::Dot(pVertex->normal, lightDir));
	
	auto viewDir = RenderContext::pMainCamera->m_position - pVertex->uv1;
	viewDir.Normalize();

	auto halfDir = viewDir + lightDir;
	halfDir.Normalize();

	Color spec = pLight->color * pow(max(0, Vector3::Dot(pVertex->normal, halfDir)), 0.5f);	// 2 gloss

	return (diffuse + spec) * pMat->GetColor(pVertex->uv.x, pVertex->uv.y);
}

ShaderLib::ShaderLib()
{
	m_vsMap["default"] = new VSProgramBase();
	m_vsMap["wave"] = new VSWave();
	m_vsMap["blin-phone"] = new VSBlinPhone();

	m_psMap["default"] = new PSProgramBase();
	m_psMap["blin-phone"] = new PSBlinPhone();
}

Shader* ShaderLib::GetShader(std::string vs, std::string ps)
{
	auto key = vs + "_" + ps;
	auto it = m_shadersMap.find(key);
	if (it != m_shadersMap.end())	
		return it->second;		
	Shader* pShader = new Shader();

	auto it1 = m_vsMap.find(vs);
	if (it1 == m_vsMap.end())
		pShader->pVSProgram = m_vsMap["default"];
	else
		pShader->pVSProgram = it1->second;

	auto it2 = m_psMap.find(ps);
	if (it2 == m_psMap.end())
		pShader->pPSProgram = m_psMap["default"];
	else
		pShader->pPSProgram = it2->second;
	
	m_shadersMap[key] = pShader;
	return pShader;

}