#include "CDevice.h"
#include "Trangle.h"

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
	auto len = screenWidth * sizeof(UINT32);
	for (int x = 0; x < screenHeight; ++x)
	{
		memset(frameBuffer[x], color, len );		
	}
}

void CDevice::ClearZBuffer()
{
	auto len = screenWidth * sizeof(float);
	for (int x = 0; x < screenHeight; ++x)
	{
		memset(frameBuffer[x], 0, len);
	}
}

void CDevice::ApplyToScreen()
{
	HDC hDC = GetDC(hMainWnd);
	BitBlt(hDC, 0, 0, screenWidth, screenHeight, hMemDc, 0, 0, SRCCOPY);
	ReleaseDC(hMainWnd, hDC);
}

void CDevice::RasterizeTrangle(Trangle* pTrangle, Material* pMat)
{}