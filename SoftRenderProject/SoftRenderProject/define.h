#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <list>

//#define CLAMP(a, min, max) a = std::min(std::max(a,min),max);
#define SAFE_DELETE(p) if(p){delete p; p = NULL;}
#define SAFE_DELETE2(p, w) if(p){ for (int i = 0 ; i < w; ++i){ delete[] p[i];} delete[] p; p = NULL;}
#define SAFE_RELEASE(p) if (p){p->Release(); delete p; p=NULL;}

#define PI 3.1415f
#define MAX_FLAT 99999
#define AngleToRad(angle) angle * PI/180.0f
#define lerp(from, to, t) to * t + from * (1 - t)
#define CHECK_FLAG(flag, p) ((1 << (int)p) & flag) > 0
#define SET_FLAG(flag, p) flag | (1 << (int)p)
#define CLEAR_FLAG(flag, p) flag & ~(1 << (int)p) 



#define ENABLE_SHADOWMAP_PCF
#define ENABLE_ADDPASS
#define ENABLE_TEX_BILINER
#define ENABLE_DEFFERED_LIGHTTING
#define ENABLE_GAMA

typedef unsigned int UINT32;


enum ENUM_FB_MODE
{
	depth = 0,
	colorBuffer = 1,
	gBuffer = 2,
};

enum RENDER_PATH
{
	forward = 0,
	defferd,
};


template <typename  T>
T Clamp(T a, T min, T max)
{	
	if (a < min)
		return min;
	if (a > max)
		return max;
	return a;	
}

class Matrix4x4;
class Vector3;
class RenderObject;
typedef std::list<RenderObject*> RENDER_LIST;

void split(std::string str, std::vector<std::string>& ret, char delim);
void TransformArray(Matrix4x4* pM, Vector3* pVArray, Vector3* ret, int size);
void CalcBounds(Vector3* pCorners, Vector3* ret, int size, float scale);
bool strEndWith(const char* str, int len1, const char* end, int len2);