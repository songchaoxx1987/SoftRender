#include "framework.h"
#include "define.h"
#include "Texture.h"
#include "Color.h"
#include "svpng.inc"
#include "lodepng.h"

bool Texture::LoadTexture(const char* path)
{
	if (strEndWith(path, strlen(path), ".bmp", 4))
		return _loadBMP(path);
	return _loadPNG(path);	
}

bool Texture::_loadBMP(const char* file)
{
	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, file, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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
			Color c((float)r / 256, (float)g / 256, (float)b / 256, gray / 255);
			textureData[i][j] = c;
		}
	}
	ReleaseDC(NULL, hdc);
	UV_STARTS_AT_TOP = false;
	return true;
}

bool Texture::_loadPNG(const char* file)
{
	std::vector<unsigned char> image; //the raw pixels
	unsigned w, h;
	unsigned error = lodepng::decode(image, w, h, file);
	if (error)
		return false;
	
	textureData = new Color * [w];
	for (int i = 0; i < w; ++i)
	{
		textureData[i] = new Color[h];
	}
	width = w;
	height = h;
	int n = 0;
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			textureData[x][y].r = (float)image[n++] / 255.0f;
			textureData[x][y].g = (float)image[n++] / 255.0f;
			textureData[x][y].b = (float)image[n++] / 255.0f;
			textureData[x][y].a = (float)image[n++] / 255.0f;
		}
	}
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
	u = CLAMP(u, 0, 1.0f);
	v = CLAMP(v, 0, 1.0f);
	if (!UV_STARTS_AT_TOP)
		v = 1.0f - v;	
	float w = (width - 1);
	float h = (height - 1);
	int intu = w * u;
	int intv = h * v;

#ifdef ENABLE_TEX_BILINER
	int du = min(intu + 1, w);
	int dv = min(intv + 1, h);

	float fu0 = (float)(intu) / (float)w;
	float fv0 = (float)(intv) / (float)h;
	float fu1 = (float)(du) / (float)w;
	float fv1 = (float)(dv) / (float)h;

	float c0 = (fu1 - fu0);
	float c1 = (fv1 - fv0);
	if (c0==0 || c1==0)
		return textureData[intu][intv];

	float fu = (u - fu0) / c0;
	float fv = (v - fv0) / c1;


	//Color z0 = textureData[intu][intv];
	//Color z1 = textureData[du][intv];
	//Color z2 = textureData[intu][dv];
	//Color z3 = textureData[du][dv];

	//Color a = lerp(z0, z1, fu);
	//Color b = lerp(z2, z3, fu);
	//Color c = lerp(a, b, fv);

	float r0 = lerp(textureData[intu][intv].r, textureData[du][intv].r, fu);
	float g0 = lerp(textureData[intu][intv].g, textureData[du][intv].g, fu);
	float b0 = lerp(textureData[intu][intv].b, textureData[du][intv].b, fu);
	float a0 = lerp(textureData[intu][intv].a, textureData[du][intv].a, fu);

	float r1 = lerp(textureData[intu][dv].r, textureData[du][dv].r, fu);
	float g1 = lerp(textureData[intu][dv].g, textureData[du][dv].g, fu);
	float b1 = lerp(textureData[intu][dv].b, textureData[du][dv].b, fu);
	float a1 = lerp(textureData[intu][dv].a, textureData[du][dv].a, fu);

	Color c;
	c.r = lerp(r0, r1, fv);
	c.g = lerp(g0, g1, fv);
	c.b = lerp(b0, b1, fv);
	c.a = lerp(a0, a1, fv);
	
#ifdef ENABLE_GAMA
	float gama = 2.2;
	c.r = pow(c.r, gama);
	c.g = pow(c.g, gama);
	c.b = pow(c.b, gama);
#endif // ENABLE_GAMA
	return c;
#else
	return textureData[intu][intv];
#endif	
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