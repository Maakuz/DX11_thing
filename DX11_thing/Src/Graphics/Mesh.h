#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <vector>
#include "Texture.h"

class Mesh 
{
public:
    Mesh(ID3D11Device* device, ID3D11DeviceContext* context, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture> textures);
    Mesh(const Mesh& mesh);
    void draw();

private:
    VertexBuffer<Vertex> m_vertexBuffer;
    IndexBuffer m_indexBuffer;
    ID3D11DeviceContext* m_deviceContext;
    std::vector<Texture> m_textures;
};
