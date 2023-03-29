#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* context, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture> textures)
{
    m_deviceContext = context;
    m_textures = textures;

    HRESULT l_hr = m_vertexBuffer.initialize(device, vertices.data(), vertices.size());
    COM_ERROR_IF_FAILED(l_hr, "Vertex buffer creation failed.");


    l_hr = m_indexBuffer.initialize(device, indices.data(), indices.size());
    COM_ERROR_IF_FAILED(l_hr, "Index buffer creation failed.");
}

Mesh::Mesh(const Mesh& mesh)
{
    m_deviceContext = mesh.m_deviceContext;
    m_indexBuffer = mesh.m_indexBuffer;
    m_vertexBuffer = mesh.m_vertexBuffer;
}

void Mesh::draw()
{
    UINT l_offset = 0;
    m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), m_vertexBuffer.StridePtr(), &l_offset);
    m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    m_deviceContext->DrawIndexed(m_indexBuffer.IndexCount(), 0, 0);
}
