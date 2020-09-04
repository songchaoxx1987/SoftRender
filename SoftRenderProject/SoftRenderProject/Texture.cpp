#include "framework.h"
#include "define.h"
#include "Texture.h"
#include "Color.h"
#include "svpng.inc"

bool Texture::LoadTexture(const char* path)
{
	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (bitmap == NULL)
		return false;
	BITMAP bmp;
	GetObject(bitmap, sizeof(BITMAP), &bmp);
	width = bmp.bmWidth;
	height = bmp.bmHeight;
	HDC hdc = CreateCompatibleDC(NULL);
	SelectObject(hdc, bitmap);

	textureData = new Color * [width];
	for (int i = 0; i < width; ++i)
	{
		textureData[i] = new Color[height];
		for (int j = 0; j < height; ++j)
		{
			COLORREF color = GetPixel(hdc, i, j);
			int r = color % 256;
			int g = (color >> 8) % 256;
			int b = (color >> 16) % 256;
			float gray = r * 0.299f + g * 0.587f + b * 0.114;			
			Color c((float)r / 256, (float)g / 256, (float)b / 256, gray/255);
			textureData[i][j] = c;
		}
	}
	ReleaseDC(NULL, hdc);
	UV_STARTS_AT_TOP = false;
	return true;
}

void Texture::CreateTexture(int w, int h, Color* pClr, bool topUv)
{
	height = h;
	width = w;
	Color c = pClr ? *pClr : Color(0, 0, 0, 1);
	textureData = new Color * [width];
	for (int i = 0; i < width; ++i)
	{
		textureData[i] = new Color[height];
		for (int j = 0; j < height; ++j)
		{			
			textureData[i][j] = c;
		}
	}
	UV_STARTS_AT_TOP = topUv;
}

void Texture::ClearTexture(Color* pClr)
{
	Color c = pClr ? *pClr : Color(0, 0, 0, 1);
	for (int i = 0; i < width; ++i)
	{	
		for (int j = 0; j < height; ++j)
		{
			textureData[i][j] = c;
		}
	}
}


Color Texture::Sample(float u, float v)
{
	u = Clamp<float>(u, 0, 1.0f);
	v = Clamp<float>(v, 0, 1.0f);
	//暂时直接采用clamp01的方式采样
	int intu = (width - 1) * u;
	if (!UV_STARTS_AT_TOP)
		v = 1.0f - v;	
	int intv = (height - 1) * v;
	return textureData[intu][intv];
}

void Texture::SetPixel(float u, float v, Color& color)
{
	u = Clamp<float>(u, 0, 1.0f);
	v = Clamp<float>(v, 0, 1.0f);	
	int intu = (width - 1) * u;	
	if (!UV_STARTS_AT_TOP)
		v = 1.0f - v;
	int intv = (height - 1) * v;
	textureData[intu][intv] = color;
}

UINT32 Texture::GetPixelColor(int x, int y)
{
	return textureData[x][y].ToRGB();
}

void Texture::Release()
{
	if (textureData == NULL)
		return;
	for (int i = 0; i < width; ++i)
	{
		delete[] textureData[i];		
	}
	delete[] textureData;
	textureData = NULL;
}

void Texture::SavePng(std::string file)
{
	unsigned char* rgba = new unsigned char[height * width * 4];		
	unsigned char* p = rgba;
	FILE* fp = fopen(file.c_str(), "wb");
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)		
		{
			auto color = textureData[x][y];
			*p++ = (unsigned char)(color.r * 255);
			*p++ = (unsigned char)(color.g * 255);
			*p++ = (unsigned char)(color.b * 255);
			*p++ = (unsigned char)(255);
		}
	}
	svpng(fp, width, height, rgba, 1);
	fclose(fp);
}