#pragma once
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
	Color operator / (const Color& right) const;
	Color operator / (float value) const;		

};

