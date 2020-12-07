#include"framework.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Matrix4x4.h"
#include "Material.h"
#include "CTimer.h"
#include "RenderContext.h"
#include "Lights.h"
#include "Camera.h"
#include "CubeMap.h"
#include "ShadowMap.h"

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
	pos.x += 0.1f * pVertex->uv.y * sin(PI * RenderContext::Time());
	
	//pos.y += 0.1f*sin(2* PI * RenderContext::Time() * pos.x * pos.z);
	pVertex->position = RenderContext::pMVP->mul(pos);
	return pVertex;
}

Vertex* VSSkyBox::Method(Vertex* pVertex)
{
	pVertex->position = RenderContext::pVP->mul(pVertex->position);	
	return pVertex;
}

Color PSProgramBase::ForwardBasePass(Vertex* pVertex, Material* pMat)
{	
	return pMat->GetTexColor(pVertex->uv.x, pVertex->uv.y) * pMat->color;
}

Color PSProgramBase::DefferdPass(Vertex* pVertex, Material* pMat)
{
	return pMat->GetTexColor(pVertex->uv.x, pVertex->uv.y) * pMat->color;
}

float PSProgramBase::AttenShadow(Vertex* pVertex)
{
	return RenderContext::pShadowMap->AttenShadow(pVertex);
}

Vertex* VSBlinPhone::Method(Vertex* pVertex)
{	
	pVertex->position = RenderContext::pMVP->mul(pVertex->position);	
	return pVertex;
}

Color PSBlinPhone::LightFunction(Light* pLight, Vertex* pVex, const Color& diffuseColor)
{
	Vector3 lightDir;
	if (pLight->mode == LightMode::directLight)
		lightDir = pLight->InvDir();
	else if (pLight->mode == LightMode::pointLight)
	{
		lightDir = pLight->transform.position - pVex->worldPos;
		lightDir.Normalize();
	}
	float atten = pLight->Atten(pVex->worldPos);
	if (atten <= 0.0001f)
		return Color(0, 0, 0, 1);
	float dot = Vector3::Dot(pVex->worldNormal, lightDir);
	Color color = pLight->color * max(0, dot);	// lambert diffuse	

	auto viewDir = RenderContext::pMainCamera->Position() - pVex->worldPos;
	viewDir.Normalize();
	auto halfDir = viewDir + lightDir;
	halfDir.Normalize();
	color *= diffuseColor;

	// spec
	Color spec = specColor;
	spec *= pLight->color;
	spec *= (pow(max(0, Vector3::Dot(pVex->worldNormal, halfDir)), 128.0f * gloss));

	color += spec;
	
	return color;

	//Color spec = pLight->color * specColor * pow(max(0, Vector3::Dot(pVex->worldNormal, halfDir)), 128.0f * gloss);
	//diffuse *= diffuseColor;
	//return diffuse + spec;
}

Color PSBlinPhone::ForwardBasePass(Vertex* pVertex, Material* pMat)
{	
	Light* pLight = (*RenderContext::m_pLights)[0];
	Color tex = pMat->GetTexColor(pVertex->uv.x, pVertex->uv.y) * pMat->color;
	return (*RenderContext::pAmbColor) * tex + LightFunction(pLight, pVertex, tex);
}

Color PSBlinPhone::ForwardAddPass(Vertex* pVertex, Material* pMat, Color& baseColor)
{
	Color ret = baseColor;
	Color tex = pMat->GetTexColor(pVertex->uv.x, pVertex->uv.y) * pMat->color;
	for (int i = 1; i < RenderContext::m_pLights->size(); ++i)
	{
		Light* pLight = (*RenderContext::m_pLights)[i];
		ret += LightFunction(pLight, pVertex, tex);
	}
	return ret;
}

Color PSBlinPhone::DefferdPass(Vertex* pVertex, Material* pMat)
{		
	Color tex = pMat->GetTexColor(pVertex->uv.x, pVertex->uv.y);
	tex *= pMat->color;
	Color ret = (*RenderContext::pAmbColor);	// amb
	ret *= tex;	
	for (int i = 0; i < RenderContext::m_pLights->size(); ++i)
	{
		Light* pLight = (*RenderContext::m_pLights)[i];
		ret += LightFunction(pLight, pVertex, tex);
	}
	return ret;
}

Color HalfLambertDiffuse::LightFunction(Light* pLight, Vertex* pVex)
{
	Vector3 lightDir;
	if (pLight->mode == LightMode::directLight)
		lightDir = pLight->InvDir();
	else if (pLight->mode == LightMode::pointLight)
	{
		lightDir = pLight->transform.position - pVex->worldPos;
		lightDir.Normalize();
	}
	float atten = pLight->Atten(pVex->worldPos);
	if (atten <= 0.0001f)
		return Color(0, 0, 0, 1);	
	float dot = Vector3::Dot(pVex->worldNormal, lightDir);
	return pLight->color * ((0.5f * dot + 1.0f) * atten);	// half lambert
}

Color HalfLambertDiffuse::ForwardBasePass(Vertex* pVertex, Material* pMat)
{	
	Light* pLight = (*RenderContext::m_pLights)[0];	
	Color diffuse = LightFunction(pLight, pVertex);
	return ((*RenderContext::pAmbColor) + diffuse) * pMat->GetTexColor(pVertex->uv.x, pVertex->uv.y) * pMat->color;
}

Color HalfLambertDiffuse::ForwardAddPass(Vertex* pVertex, Material* pMat, Color& baseColor)
{
	Color add(0,0,0,1);
	for (int i = 1; i < RenderContext::m_pLights->size(); ++i)
	{
		Light* pLight = (*RenderContext::m_pLights)[i];
		add += LightFunction(pLight, pVertex);
	}
	add += baseColor;
	return add * pMat->GetTexColor(pVertex->uv.x, pVertex->uv.y) * pMat->color;
}

Color HalfLambertDiffuse::DefferdPass(Vertex* pVertex, Material* pMat)
{	
	Color col(0, 0, 0, 1); 
	for (int i = 0; i < RenderContext::m_pLights->size(); ++i)
	{
		Light* pLight = (*RenderContext::m_pLights)[i];
		col += LightFunction(pLight, pVertex);
	}
	col += (*RenderContext::pAmbColor);
	col *= pMat->GetTexColor(pVertex->uv.x, pVertex->uv.y);
	col *= pMat->color;
	return col;
}

Color PSSkyBox::ForwardBasePass(Vertex* pVertex, Material* pMat)
{
	return GetRet(pVertex, pMat);
}

Color PSSkyBox::DefferdPass(Vertex* pVertex, Material* pMat)
{	
	return GetRet(pVertex,pMat);
}

Color PSSkyBox::GetRet(Vertex* pVertex, Material* pMat)
{
	return pMat->pCubeMap->Sample(pVertex->position.x, pVertex->position.y, pVertex->position.z) * pMat->color;
}

ShaderLib::ShaderLib()
{
	m_vsMap["default"] = new VSProgramBase();
	m_vsMap["wave"] = new VSWave();
	m_vsMap["blin-phone"] = new VSBlinPhone();
	m_vsMap["skybox"] = new VSSkyBox();

	m_psMap["default"] = new PSProgramBase();
	m_psMap["blin-phone"] = new PSBlinPhone();
	m_psMap["diffuse"] = new HalfLambertDiffuse();
	m_psMap["skybox"] = new PSSkyBox();
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