#pragma once
#include "framework.h"
class Texture;
class CDevice;
class Vertex;
class Material;

class GPixel
{
public:
	GPixel();
	~GPixel();
	void Reset();
	Vertex *pVertex;
	Material* pMaterial;
	bool usedFlag;
};

class FrameBuffer
{
public:
	void ApplyToDevice(CDevice* pDevice);
	void Create(int w, int h, int flag);
	void Clear(float r, float g, float b, float a, float dep);
	void Release();

	bool ZTest(int x, int y, float z)
	{
		if (!m_zBuffer)
			return true;		
		return m_zBuffer[y][x] <= z;
	}

	void ZWrite(int x, int y, float z)
	{
		if (m_zBuffer)
			m_zBuffer[y][x] = z;
	}

	float depth(int x, int y) { return m_zBuffer[y][x]; }

	float width() { return m_width; }
	float height() { return m_height; }

	bool isFrameBufferAble() { return m_pFrameBuffer != NULL; }
	bool isGBufferAble() { return m_pGBuffer != NULL; }
	bool isDepthAble() { return m_zBuffer != NULL; }

	Texture* GetFrameBuffTex() { return m_pFrameBuffer; }
protected:
	float m_width;
	float m_height;

	Texture* m_pFrameBuffer;
	GPixel** m_pGBuffer;
	float** m_zBuffer;
};

