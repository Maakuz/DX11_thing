#include "Camera.h"
using namespace DirectX;

Camera::Camera()
{

    this->updateView();
}

void Camera::setProjecton(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    float l_fovRad = (fovDegrees / 360.f) * XM_2PI;
    m_projection = XMMatrixPerspectiveFovLH(l_fovRad, aspectRatio, nearZ, farZ);
}

void Camera::setPos(const DirectX::XMVECTOR& pos)
{
    XMStoreFloat3(&m_pos, pos);
    m_posVec = pos;
    this->updateView();
}

void Camera::setPos(const DirectX::XMFLOAT3& pos)
{
    m_pos = pos;
    m_posVec = XMLoadFloat3(&m_pos);
    this->updateView();
}

void Camera::setPos(float x, float y, float z)
{
    m_pos = XMFLOAT3(x, y, z);
    m_posVec = XMLoadFloat3(&m_pos);
    this->updateView();
}

void Camera::move(const DirectX::XMVECTOR& pos)
{
    m_posVec += pos;
    XMStoreFloat3(&m_pos, m_posVec);
    this->updateView();
}

void Camera::move(const DirectX::XMFLOAT3& pos)
{
    m_pos.x += pos.x;
    m_pos.y += pos.y;
    m_pos.z += pos.z;
    m_posVec = XMLoadFloat3(&m_pos);
    this->updateView();
}

void Camera::move(float x, float y, float z)
{
    m_pos.x += x;
    m_pos.y += y;
    m_pos.z += z;
    m_posVec = XMLoadFloat3(&m_pos);
    this->updateView();
}

void Camera::setRotation(const DirectX::XMVECTOR& rot)
{
    XMStoreFloat3(&m_rotation, rot);
    m_posVec = rot;
    this->updateView();
}

void Camera::setRotation(const DirectX::XMFLOAT3& rot)
{
    m_rotation = rot;
    m_RotationVec = XMLoadFloat3(&m_rotation);
    this->updateView();
}

void Camera::setRotation(float x, float y, float z)
{
    m_rotation = XMFLOAT3(x, y, z);
    m_RotationVec = XMLoadFloat3(&m_rotation);
    this->updateView();
}

void Camera::rotate(const DirectX::XMVECTOR& rot)
{
    m_RotationVec += rot;
    XMStoreFloat3(&m_rotation, m_RotationVec);
    this->updateView();
}

void Camera::rotate(const DirectX::XMFLOAT3& rot)
{
    m_rotation.x += rot.x;
    m_rotation.y += rot.y;
    m_rotation.z += rot.z;
    m_RotationVec = XMLoadFloat3(&m_rotation);
    this->updateView();
}

void Camera::rotate(float x, float y, float z)
{
    m_rotation.x += x;
    m_rotation.y += y;
    m_rotation.z += z;
    m_RotationVec = XMLoadFloat3(&m_rotation);
    this->updateView();
}

void Camera::setLookAt(DirectX::XMFLOAT3 lookAt)
{
    lookAt.x = m_pos.x - lookAt.x;
    lookAt.y = m_pos.y - lookAt.y;
    lookAt.z = m_pos.z - lookAt.z;

    if (lookAt.x < FLT_EPSILON && lookAt.y < FLT_EPSILON && lookAt.z < FLT_EPSILON)
        return;

    float l_pitch = 0.f;
    if (lookAt.y > FLT_EPSILON)
    {
        float l_distance = sqrt((lookAt.x * lookAt.x) + (lookAt.z * lookAt.z));
        l_pitch = atan(lookAt.y / l_distance);
    }

    float l_yaw = 0.f;
    if (lookAt.x > FLT_EPSILON)
        l_yaw = atan(lookAt.x / lookAt.z);

    if (lookAt.z > FLT_EPSILON)
        l_yaw += XM_PI;

    this->setRotation(l_pitch, l_yaw, 0);
}

void Camera::updateView()
{
    XMMATRIX l_rotMat = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
    XMVECTOR l_target = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, l_rotMat);
    l_target += m_posVec;

    XMVECTOR l_up = XMVector3TransformCoord(DEFAULT_UP_VECTOR, l_rotMat);
    m_view = XMMatrixLookAtLH(m_posVec, l_target, l_up);

    XMMATRIX l_vecRotMat = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, 0.f);
    m_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, l_vecRotMat);
    m_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, l_vecRotMat);
}
