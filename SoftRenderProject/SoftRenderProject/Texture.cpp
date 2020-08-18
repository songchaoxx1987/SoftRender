#include "framework.h"
#include "define.h"
#include "Texture.h"
#include "Color.h"

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
			//float gray = r * 0.299f + g * 0.587f + b * 0.114;			
			Color c((float)r / 256, (float)g / 256, (float)b / 256, 1/* gray/255*/);
			textureData[i][j] = c;
		}
	}
	ReleaseDC(NULL, hdc);
	return true;
}

Color Texture::Sample(float u, float v)
{
	u = Clamp<float>(0, 1.0f, u);
	v = Clamp<float>(0, 1.0f, v);
	//��ʱֱ�Ӳ���clamp01�ķ�ʽ����
	int intu = (width - 1) * u;
	int intv = (height - 1) * v;
	return textureData[intu][intv];
}