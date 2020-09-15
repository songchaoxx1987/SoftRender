#include "Color.h"


#define COLOR_ADD(a,b) Clamp<float>(a + b, 0.0f ,1.0f)
#define COLOR_MUL(a,b) Clamp<float>(a * b, 0.0f ,1.0f)
#define COLOR_DIV(a,b) Clamp<float>(a / b, 0.0f ,1.0f)

Color Color::operator + (const Color& right) const
{
	Color ret;
	ret.r = COLOR_ADD(r, right.r);
	ret.g = COLOR_ADD(g, right.g);
	ret.b = COLOR_ADD(b, right.b);
	ret.a = COLOR_ADD(a, right.a);
	return ret;
}

Color Color::operator - (const Color& right) const
{
	Color ret;
	ret.r = COLOR_ADD(r, -right.r);
	ret.g = COLOR_ADD(g, -right.g);
	ret.b = COLOR_ADD(b, -right.b);
	ret.a = COLOR_ADD(a, -right.a);
	return ret;
}

Color Color::operator * (float value) const
{
	Color ret;
	ret.r = COLOR_MUL(r, value);
	ret.g = COLOR_MUL(g, value);
	ret.b = COLOR_MUL(b, value);
	ret.a = COLOR_MUL(a, value);
	return ret;
}

Color Color::operator / (float value) const
{
	Color ret;
	ret.r = COLOR_DIV(r, value);
	ret.g = COLOR_DIV(g, value);
	ret.b = COLOR_DIV(b, value);
	ret.a = COLOR_DIV(a, value);
	return ret;
}

Color Color::operator * (const Color& right) const
{
	Color ret;
	ret.r = COLOR_MUL(r, right.r);
	ret.g = COLOR_MUL(g, right.g);
	ret.b = COLOR_MUL(b, right.b);
	ret.a = COLOR_MUL(a, right.a);
	return ret;
}

Color Color::operator / (const Color& right) const
{
	Color ret;
	ret.r = COLOR_DIV(r, right.r);
	ret.g = COLOR_DIV(g, right.g);
	ret.b = COLOR_DIV(b, right.b);
	ret.a = COLOR_DIV(a, right.a);
	return ret;
}

void Color::operator += (const Color& right)
{	
	r = COLOR_ADD(r, right.r);
	g = COLOR_ADD(g, right.g);
	b = COLOR_ADD(b, right.b);
	a = COLOR_ADD(a, right.a);
}

void Color::operator *= (const Color& right) 
{	
	r = COLOR_MUL(r, right.r);
	g = COLOR_MUL(g, right.g);
	b = COLOR_MUL(b, right.b);
	a = COLOR_MUL(a, right.a);
}

void Color::operator *= (float value) 
{	
	r = COLOR_MUL(r, value);
	g = COLOR_MUL(g, value);
	b = COLOR_MUL(b, value);
	a = COLOR_MUL(a, value);	
}

UINT32 Color::ToRGB()
{
	return ToRGB(r, g, b);
}

UINT32 Color::ToRGB(float fr, float fg, float fb)
{
	int R = (int)(fr * 255.0f);
	int G = (int)(fg * 255.0f);
	int B = (int)(fb * 255.0f);
	R = Clamp(R, 0, 255);
	G = Clamp(G, 0, 255);
	B = Clamp(B, 0, 255);
	return (R << 16) | (G << 8) | (B);
}
