#include "Color.h"
#include "define.h"

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
