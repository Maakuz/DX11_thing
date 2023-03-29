#include "GameObject.h"
using namespace DirectX;

bool GameObject::initialize(std::string filePath, ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<ConstantBuffers::VS_vertexshader>& VS_CB)
{
    if (!m_model.initialize(filePath, device, context, VS_CB))
        return false;

    this->setPos(0, 0, 0);
    this->setRotation(0, 0, 0);

    this->updateWorld();

    return true;
}

void GameObject::draw(const DirectX::XMMATRIX& vp)
{
    m_model.draw(m_world, vp);
}

void GameObject::setPos(const DirectX::XMVECTOR& pos)
{
    XMStoreFloat3(&m_pos, pos);
    m_posVec = pos;
    updateWorld();
}

void GameObject::setPos(const DirectX::XMFLOAT3& pos)
{
    m_pos = pos;
    m_posVec = XMLoadFloat3(&m_pos);
    updateWorld();
}

void GameObject::setPos(float x, float y, float z)
{
    m_pos = XMFLOAT3(x, y, z);
    m_posVec = XMLoadFloat3(&m_pos);
    updateWorld();
}

void GameObject::move(const DirectX::XMVECTOR& pos)
{
    m_posVec += pos;
    XMStoreFloat3(&m_pos, m_posVec);
    updateWorld();
}

void GameObject::move(const DirectX::XMFLOAT3& pos)
{
    m_pos.x += pos.x;
    m_pos.y += pos.y;
    m_pos.z += pos.z;
    m_posVec = XMLoadFloat3(&m_pos);
    updateWorld();
}

void GameObject::move(float x, float y, float z)
{
    m_pos.x += x;
    m_pos.y += y;
    m_pos.z += z;
    m_posVec = XMLoadFloat3(&m_pos);
    updateWorld();
}

void GameObject::setRotation(const DirectX::XMVECTOR& rot)
{
    XMStoreFloat3(&m_rotation, rot);
    m_posVec = rot;
    updateWorld();
}

void GameObject::setRotation(const DirectX::XMFLOAT3& rot)
{
    m_rotation = rot;
    m_RotationVec = XMLoadFloat3(&m_rotation);
    updateWorld();
}

void GameObject::setRotation(float x, float y, float z)
{
    m_rotation = XMFLOAT3(x, y, z);
    m_RotationVec = XMLoadFloat3(&m_rotation);
    updateWorld();
}

void GameObject::rotate(const DirectX::XMVECTOR& rot)
{
    m_RotationVec += rot;
    XMStoreFloat3(&m_rotation, m_RotationVec);
    updateWorld();
}

void GameObject::rotate(const DirectX::XMFLOAT3& rot)
{
    m_rotation.x += rot.x;
    m_rotation.y += rot.y;
    m_rotation.z += rot.z;
    m_RotationVec = XMLoadFloat3(&m_rotation);
    updateWorld();
}

void GameObject::rotate(float x, float y, float z)
{
    m_rotation.x += x;
    m_rotation.y += y;
    m_rotation.z += z;
    m_RotationVec = XMLoadFloat3(&m_rotation);
    updateWorld();
}

void GameObject::updateWorld()
{
    m_world = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z) * XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);

    XMMATRIX l_vecRotMat = XMMatrixRotationRollPitchYaw(0.f, m_rotation.y, 0.f);

    m_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, l_vecRotMat);
    m_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, l_vecRotMat);
}