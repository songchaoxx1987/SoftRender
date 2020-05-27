#include"define.h"
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"CMesh.h"
#include"Vector3.h"
#include"Vector2.h"
#include"Vertex.h"


void CMesh::LoadFromFile(const char* file)
{
	std::ifstream stream;
	stream.open(file);
	std::string line;

	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;
	std::vector<std::string> temp;

	std::vector<int> posIdx;
	std::vector<int> normalIdx;
	std::vector<int> uvIdx;

	while (getline(stream, line))
	{	
		temp.clear();
		split(line, temp, ' ');
		if (temp.size() == 0)
			continue;
		auto key = temp[0];
		if (key == "v")
		{
			Vector3 pos;
			pos.x = atof(temp[1].c_str());
			pos.y = atof(temp[2].c_str());
			pos.z = atof(temp[3].c_str());
			positions.push_back(pos);
			pos.Print();
		}
		else if(key=="vn")
		{
			Vector3 n;
			n.x = atof(temp[1].c_str());
			n.y = atof(temp[2].c_str());
			n.z = atof(temp[3].c_str());
			normals.push_back(n);
			n.Print();
		}
		else if(key=="vt")
		{
			Vector2 uv;
			uv.x = atof(temp[1].c_str());
			uv.y = atof(temp[2].c_str());
			uvs.push_back(uv);
			uv.Print();
		}
		else if (key=="f")
		{
			std::vector<std::string> v;
			for (int i = 1; i <= 3; ++i)
			{
				v.clear();
				split(temp[i],v,'/');				
				posIdx.push_back(atoi(v[0].c_str()));
				uvIdx.push_back(atoi(v[1].c_str()));
				normalIdx.push_back(atoi(v[2].c_str()));

				int idx = posIdx.size() - 1;
				printf("%d %d %d\n", posIdx[idx], uvIdx[idx], normalIdx[idx]);
			}
			
		}
	}
	stream.close();	
	pVextexs = new Vertex[posIdx.size()];
	for (int i = 0; i < posIdx.size() ; ++i)
	{
		pVextexs[i].position = positions[posIdx[i] - 1];
		pVextexs[i].normal = normals[normalIdx[i] - 1];
		pVextexs[i].uv = uvs[uvIdx[i] - 1];
	}

}

void CMesh::Release()
{
	SAFE_DELETE(pVextexs);
}