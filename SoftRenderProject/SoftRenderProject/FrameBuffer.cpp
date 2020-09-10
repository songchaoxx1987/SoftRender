#include "FrameBuffer.h"
#include "define.h"
#include "Vertex.h"
#include "Texture.h"
#include "CDevice.h"

GPixel::GPixel()
{
	pVertex = new Vertex();
	pMaterial = NULL;
	usedFlag = false;
}

GPixel::~GPixel()
{
	SAFE_DELETE(pVertex);
	pMaterial = NULL;
}

void GPixel::Reset()
{	
	pMaterial = NULL;
	usedFlag = false;
}

void FrameBuffer::Create(int w, int h, int flag)
{
	m_width = w;
	m_height = h;
	if (CHECK_FLAG(flag, ENUM_FB_MODE::depth))
	{
		m_zBuffer = new float* [m_height];
		for (int i = 0; i < m_height; ++i)
			m_zBuffer[i] = new float[m_width];
	}

	bool needFrameBuffer = false;
	SAFE_DELETE2(m_pGBuffer, m_height);
	if (CHECK_FLAG(flag, ENUM_FB_MODE::gBuffer))
	{
		needFrameBuffer = true;
		m_pGBuffer = new GPixel * [m_height];
		for (int i = 0; i < m_height; ++i)
			m_pGBuffer[i] = new GPixel[m_width];
	}

	SAFE_RELEASE(m_pFrameBuffer);
	if (needFrameBuffer || CHECK_FLAG(flag, ENUM_FB_MODE::colorBuffer))
	{
		m_pFrameBuffer = new Texture();
		m_pFrameBuffer->CreateTexture(m_width, m_height, NULL, false);		
	}

	
}

void FrameBuffer::Clear(float r, float g, float b, float a, float dep)
{
	if (m_pFrameBuffer != NULL)
	{
		Color c = Color(r, g, b, a);
		m_pFrameBuffer->ClearTexture(&c);
	}

	if (m_pGBuffer || m_zBuffer)
	{
		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x)
			{
				if (m_pGBuffer)
					m_pGBuffer[y][x].Reset();
				if (m_zBuffer)
					m_zBuffer[y][x] = dep;
			}

		}
	}
}


void FrameBuffer::Release()
{
	SAFE_DELETE2(m_pGBuffer, m_height);
	SAFE_DELETE2(m_zBuffer, m_height);
	SAFE_RELEASE(m_pFrameBuffer);
}

void FrameBuffer::ApplyToDevice(CDevice* pDevice)
{
	if (!m_pFrameBuffer || !pDevice)
		return;
	for (int y = 0; y < pDevice->screenHeight; ++y)
	{
		for (int x = 0; x < pDevice->screenWidth; ++x)
		{
			UINT32 c = m_pFrameBuffer->GetPixelColor(x, y);
			pDevice->DrawPiexl(x, y, c);
		}
	}
}

float FrameBuffer::SampleDepth(float u, float v, bool bilinear)
{
	int x = (int)u;
	int y = (int)v;
	if (bilinear)
	{
		float fx = u - x;
		float fy = v - y;

		int dy = min(y + 1, m_height);
		int dx = min(x + 1, m_width);

		float z0 = m_zBuffer[y][x];
		float z1 = m_zBuffer[y][dx];
		float z2 = m_zBuffer[dy][x];
		float z3 = m_zBuffer[dy][dx];

		float a = lerp(z0, z1, fx);
		float b = lerp(z2, z3, fx);
		float c = lerp(a, b, fy);
		return c;
	}
	return m_zBuffer[y][x];
}