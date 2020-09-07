#include "define.h"
#include "Matrix4x4.h"
#include "Vector3.h"

void split(std::string str, std::vector<std::string>& ret, char delim)
{
	size_t begin = 0;
	size_t i = 0;
	while (i < str.length())
	{
		if (str[i] == delim)
		{
			int cnt = i - begin;
			if (cnt > 0)
			{
				std::string temp;
				temp = str.substr(begin, i - begin);
				ret.push_back(temp);
			}
			begin = i + 1;
		}
		++i;
	}
	if (i > begin)
	{
		std::string temp;
		temp = str.substr(begin, i - begin);
		ret.push_back(temp);
	}
}

bool strEndWith(const char* str, int len1, const char* end, int len2)
{
	if (len1 < len2)
		return false;
	int n = len2;
	const char* p1 = str + len1 - 1;
	const char* p2 = end + len2 - 1;
	while (*p2 == *p1 && n> 0)
	{
		--p2;
		--p1;
		--n;
	}
	return n == 0;
}

void TransformArray(Matrix4x4* pM, Vector3* pVArray, Vector3* ret, int size)
{
	for (int i = 0; i < size; ++i)
	{
		ret[i] = pM->mul(pVArray[i]);
	}
}

void CalcBounds(Vector3* pCorners, Vector3* ret, int size, float scale)
{
	ret[0].x = ret[0].y = ret[0].z = MAX_FLAT;
	ret[1].x = ret[1].y = ret[1].z = -MAX_FLAT;
	for (int i = 0; i < size; ++i)
	{
		ret[0] = Vector3::Min(pCorners[i], ret[0]);
		ret[1] = Vector3::Max(pCorners[i], ret[1]);
	}
	ret[0] = ret[0] * scale;
	ret[1] = ret[1] * scale;
}