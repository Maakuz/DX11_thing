#include "Model.h"

using namespace DirectX;

bool Model::initialize(std::string filePath, ID3D11Device* device, ID3D11DeviceContext* context, ID3D11ShaderResourceView* texture, ConstantBuffer<ConstantBuffers::VS_vertexshader>& VS_CB)
{
    m_device = device;
    m_deviceContext = context;
    m_texture = texture;
    m_constantBuffer = &VS_CB;

    this->setPos(0, 0, 0);
    this->setRotation(0, 0, 0);

    this->updateWorld();

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

void Model::setTexture(ID3D11ShaderResourceView* texture)
{
    m_texture = texture;
}

void Model::draw(const DirectX::XMMATRIX& vp)
{
    m_constantBuffer->m_data.mat = m_world * vp;
    m_constantBuffer->m_data.mat = XMMatrixTranspose(m_constantBuffer->m_data.mat);
    m_constantBuffer->UpdateBuffer();

    m_deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer->GetAddressOf());
    
    m_deviceContext->PSSetShaderResources(0, 1, &m_texture);

    for (Mesh& mesh : m_meshes)
        mesh.draw();
}

void Model::setPos(const DirectX::XMVECTOR& pos)
{
    XMStoreFloat3(&m_pos, pos);
    m_posVec = pos;
    updateWorld();
}

void Model::setPos(const DirectX::XMFLOAT3& pos)
{
    m_pos = pos;
    m_posVec = XMLoadFloat3(&m_pos);
    updateWorld();
}

void Model::setPos(float x, float y, float z)
{
    m_pos = XMFLOAT3(x, y, z);
    m_posVec = XMLoadFloat3(&m_pos);
    updateWorld();
}

void Model::move(const DirectX::XMVECTOR& pos)
{
    m_posVec += pos;
    XMStoreFloat3(&m_pos, m_posVec);
    updateWorld();
}

void Model::move(const DirectX::XMFLOAT3& pos)
{
    m_pos.x += pos.x;
    m_pos.y += pos.y;
    m_pos.z += pos.z;
    m_posVec = XMLoadFloat3(&m_pos);
    updateWorld();
}

void Model::move(float x, float y, float z)
{
    m_pos.x += x;
    m_pos.y += y;
    m_pos.z += z;
    m_posVec = XMLoadFloat3(&m_pos);
    updateWorld();
}

void Model::setRotation(const DirectX::XMVECTOR& rot)
{
    XMStoreFloat3(&m_rotation, rot);
    m_posVec = rot;
    updateWorld();
}

void Model::setRotation(const DirectX::XMFLOAT3& rot)
{
    m_rotation = rot;
    m_RotationVec = XMLoadFloat3(&m_rotation);
    updateWorld();
}

void Model::setRotation(float x, float y, float z)
{
    m_rotation = XMFLOAT3(x, y, z);
    m_RotationVec = XMLoadFloat3(&m_rotation);
    updateWorld();
}

void Model::rotate(const DirectX::XMVECTOR& rot)
{
    m_RotationVec += rot;
    XMStoreFloat3(&m_rotation, m_RotationVec);
    updateWorld();
}

void Model::rotate(const DirectX::XMFLOAT3& rot)
{
    m_rotation.x += rot.x;
    m_rotation.y += rot.y;
    m_rotation.z += rot.z;
    m_RotationVec = XMLoadFloat3(&m_rotation);
    updateWorld();
}

void Model::rotate(float x, float y, float z)
{
    m_rotation.x += x;
    m_rotation.y += y;
    m_rotation.z += z;
    m_RotationVec = XMLoadFloat3(&m_rotation);
    updateWorld();
}

void Model::updateWorld()
{
    m_world = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z) * XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);

    XMMATRIX l_vecRotMat = XMMatrixRotationRollPitchYaw(0.f, m_rotation.y, 0.f);

    m_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, l_vecRotMat);
    m_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, l_vecRotMat);
}
