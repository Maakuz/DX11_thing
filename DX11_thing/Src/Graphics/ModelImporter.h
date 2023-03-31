#pragma once
#include "Vertex.h"
#include <vector>
#include <d3d11.h>
#include "../ErrorLogger.h"
#include "Mesh.h"
#include "Material.h"

//Test multimaterial import (currently exporter is not supporting it)
//Test texture import

class ModelImporter
{
public:
    struct MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<DWORD> indices;
        std::vector<int> materialIndices;
        std::vector<int> materialUsed;
        std::vector<Material> materials;
    };

	static std::vector<Mesh> loadObjModel(std::string path, ID3D11Device* device, ID3D11DeviceContext* context);
	static std::vector<Mesh> loadModel(std::string path, ID3D11Device* device, ID3D11DeviceContext* context);

private:
};

