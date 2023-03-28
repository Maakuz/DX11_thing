#pragma once
#include "Vertex.h"
#include <vector>
#include <d3d11.h>
#include "../ErrorLogger.h"

class ModelImporter
{
public:
	struct Data
	{
		std::vector<Vertex> vertices;
		std::vector<DWORD> indices;
	};

	static Data loadObjModel(std::string path);
private:
	struct Pos
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};

	struct UV
	{
		float u = 0;
		float v = 0;
	};
};

