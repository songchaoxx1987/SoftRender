#include "CDevice.h"
#include "Trangle.h"
#include "Material.h"
#include "Texture.h"

#define MAX_FLAT 99999

//#define I(a,b) a.y - b.y
//#define J(a,b) b.x - a.x
//#define F(a,b) a.x * b.y - b.x * a.y



void CDevice::Init(HWND hWnd, int screenW, int screenH)
{
	hMainWnd = hWnd;
	HDC hDC = GetDC(hMainWnd);
	hMemDc = CreateCompatibleDC(hDC);
	ReleaseDC(hMainWnd, hDC);

	screenWidth = screenW;
	screenHeight = screenH;

	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), screenWidth, -screenHeight, 1, 32, BI_RGB,
		screenWidth* screenHeight * 4, 0, 0, 0, 0 } };
	
	LPVOID ptr;
	hBMP = CreateDIBSection(hMemDc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);

	hOldBMP = (HBITMAP)SelectObject(hMemDc, hBMP);
	unsigned char* fb = (unsigned char*)ptr;
	memset(fb, 0, screenWidth * screenHeight * 4);

	frameBuffer = new UINT32 * [screenHeight];
	for (int j = 0; j < screenHeight; j++) {
		frameBuffer[j] = (UINT32*)(fb + screenWidth * 4 * j);
	}

	zBuffer = new float* [screenHeight];
	for (int i = 0; i < screenHeight; ++i)
		zBuffer[i] = new float[screenWidth];

}

void CDevice::DrawPiexl(int x, int y, UINT32 color)
{
	if (x < 0 ||
		x > screenWidth ||
		y< 0 ||
		y > screenHeight
		)
		return;
	frameBuffer[y][x] = color;
	
}

void CDevice::Clear(UINT32 color)
{		
	for (int y = 0; y < screenHeight; ++y)
	{
		for (int x = 0; x < screenWidth; ++x)
		{
			frameBuffer[y][x] = color;
			zBuffer[y][x] = MAX_FLAT; 
		}
		
	}
}

void CDevice::ClearZBuffer()
{
	auto len = screenWidth * sizeof(float);
	for (int y = 0; y < screenHeight; ++y)
	{
		for (int x = 0; x < screenWidth; ++x)
			zBuffer[y][x] = MAX_FLAT;
	}
}

void CDevice::ApplyToScreen()
{
	HDC hDC = GetDC(hMainWnd);
	BitBlt(hDC, 0, 0, screenWidth, screenHeight, hMemDc, 0, 0, SRCCOPY);
	ReleaseDC(hMainWnd, hDC);
}


void CDevice::RasterizeTrangle(Trangle* pTrangle, Material* pMat)
{
	pTrangle->calcBounds();

	int minX = pTrangle->bounds[0].x;
	int maxX = pTrangle->bounds[1].x;
	int minY = pTrangle->bounds[0].y;
	int maxY = pTrangle->bounds[1].y;
	
	// ��������ϵ����ʱ��	
	Vector2 ab = pTrangle->v[1].position - pTrangle->v[0].position;
	Vector2 bc = pTrangle->v[2].position - pTrangle->v[1].position;
	Vector2 ca = pTrangle->v[0].position - pTrangle->v[2].position;	
	
	float areaTrangle2 = abs(Vector2::Cross(ab, bc));
	if (areaTrangle2 <= 0)
		return;
	for (int y = minY; y <= maxY; ++y)
	{
		bool inside = false;
		for (int x = minX; x <= maxX; ++x)
		{
			++pixelCnt;
			Vector3 p(x, y, 0);
			Vector2 ap = p - pTrangle->v[0].position;
			Vector2 bp = p - pTrangle->v[1].position;
			Vector2 cp = p - pTrangle->v[2].position;

			float c1 = Vector2::Cross(ab, ap);
			float c2 = Vector2::Cross(bc, bp);
			float c3 = Vector2::Cross(ca, cp);
			if (/*(c1 >= 0 && c2 >= 0 && c3 >= 0) || */(c1 <= 0 && c2 <= 0 && c3 <= 0))
			{
				inside = true;

				float lamda1 = abs(c2 / areaTrangle2) / pTrangle->v[0].position.z;
				float lamda2 = abs(c3 / areaTrangle2) / pTrangle->v[1].position.z;
				float lamda3 = abs(c1 / areaTrangle2) / pTrangle->v[2].position.z;

				//float lamda1 = abs(c2 / areaTrangle2) * pTrangle->v[0].rhw;
				//float lamda2 = abs(c3 / areaTrangle2) * pTrangle->v[1].rhw;
				//float lamda3 = abs(c1 / areaTrangle2) * pTrangle->v[2].rhw;

				float rhw = lamda1 + lamda2 + lamda3;		
				float z = 1.0f / rhw;
				if (pMat->zTest && zBuffer[y][x] < z)
					continue;
				if (pMat->zWrite)
					zBuffer[y][x] = z;
				++pixelRealCnt;
				Vector2 uv = (pTrangle->v[0].uv * lamda1 + pTrangle->v[1].uv * lamda2 + pTrangle->v[2].uv * lamda3) * z;
				UINT32 rgb = pMat->GetColor(uv.x, uv.y);					
				DrawPiexl(x, y, rgb);
			}
			else
			{
				if (inside)
					break;
			}		
		}
	}
}