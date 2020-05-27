#pragma once
#include "stdio.h"

class Vertex;
class CMesh
{
public:
	Vertex* pVextexs = NULL;

	void LoadFromFile(const char* file);
	void Release();
};