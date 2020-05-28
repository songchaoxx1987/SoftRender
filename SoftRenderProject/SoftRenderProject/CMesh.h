#pragma once
#include "stdio.h"

class Vertex;
class CMesh
{
public:
	~CMesh()
	{
		Release();
	}

	Vertex* m_pVextexs = NULL;
	int m_vextexCnt;

	void LoadFromFile(const char* file);
	void Release();
};