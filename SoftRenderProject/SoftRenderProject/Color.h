#pragma once
#include "define.h"
struct Color
{
public:
	float r, g, b, a;
	Color()
	{
		r = b = g = 0;
		a = 1;
	}

	Color(float fr,float fg, float fb)
	{
		r = fr;
		g = fg;
		b = fb;
		a = 1;
	}

	Color(float fr, float fg, float fb, float fa)
	{
		r = fr;
		g = fg;
		b = fb;
		a = fa;
	}

	Color operator + (const Color& right) const;
	Color operator - (const Color& right) const;
	Color operator * (const Color& right) const;
	Color operator * (float value) const;
	void operator += (const Color& right);
	void operator *= (const Color& right);
	void operator *= (float value);
	Color operator / (const Color& right) const;
	Color operator / (float value) const;		
	UINT32 ToRGB();
	static UINT32 ToRGB(float fr, float fg, float fb);
	

};

