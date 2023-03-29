#pragma once
#include "Mesh.h"
#include "ModelImporter.h"

class Model
{
public:
    bool initialize(std::string filePath, ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<ConstantBuffers::VS_vertexshader>& VS_CB);
    void draw(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& vp);

private:
    std::vector<Mesh> m_meshes;

    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_deviceContext = nullptr;
    ConstantBuffer<ConstantBuffers::VS_vertexshader>* m_constantBuffer = nullptr;
};