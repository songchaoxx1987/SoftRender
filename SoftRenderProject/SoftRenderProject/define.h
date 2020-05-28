#pragma once

#include<algorithm>
#include<string>
#include<vector>

//#define CLAMP(a, min, max) a = std::min(std::max(a,min),max);
#define SAFE_DELETE(p) if(p){delete p; p = NULL;}

#define PI 3.1415f
#define AngleToRad(angle) angle * PI/180.0f
#define lerp(from, to, t)  to * t + (1 - t) * from




template <typename  T>
T Clamp(T a, T min, T max)
{	
	if (a < min)
		return min;
	if (a > max)
		return max;
	return a;	
}


//template <typename  T>
//T Lerp(const T& from, const T& to, float t)
//{
//	return to * t + from * (1 - t);
//}

void split(std::string str, std::vector<std::string>& ret, char delim);
