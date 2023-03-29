#include "Model.h"
using namespace DirectX;

bool Model::initialize(std::string filePath, ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<ConstantBuffers::VS_vertexshader>& VS_CB)
{
    m_device = device;
    m_deviceContext = context;
    m_constantBuffer = &VS_CB;

    ModelImporter::Data l_data = ModelImporter::loadObjModel(filePath);

    try 
    {
        m_meshes.push_back(Mesh(m_device, m_deviceContext, l_data.vertices, l_data.indices));
    }
    catch (COMException& e)
    {
        ErrorLogger::log(e);
        return false;
    }

    return true;
}

void Model::draw(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& vp)
{
    m_constantBuffer->m_data.mat = world * vp;
    m_constantBuffer->m_data.mat = XMMatrixTranspose(m_constantBuffer->m_data.mat);
    m_constantBuffer->UpdateBuffer();

    m_deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer->GetAddressOf());

    for (Mesh& mesh : m_meshes)
        mesh.draw();
}


