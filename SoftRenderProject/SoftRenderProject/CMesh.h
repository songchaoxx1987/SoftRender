#pragma once
#include "stdio.h"
#include <string>
#include"ResourcesBase.h"

class Vertex;
class CMesh : public ResourcesBase
{
public:
	~CMesh()
	{
		Release();
	}

	std::string file;
	Vertex* m_pVextexs = NULL;
	int m_vextexCnt;

	void LoadFromFile(const char* file);
	void Release();
};