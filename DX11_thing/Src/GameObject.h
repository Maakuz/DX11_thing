#pragma once
#include "Graphics/Model.h"

class GameObject
{
public:
    bool initialize(std::string filePath, ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<ConstantBuffers::VS_vertexshader>& VS_CB);

    void draw(const DirectX::XMMATRIX& vp);

    const DirectX::XMVECTOR& getPosVec() const { return m_posVec; }
    const DirectX::XMFLOAT3& getPosFloat3() const { return m_pos; }
    const DirectX::XMVECTOR& getRotationVec() const { return m_RotationVec; }
    const DirectX::XMFLOAT3& getRotationFloat3() const { return m_rotation; }

    const DirectX::XMVECTOR& getForward() const { return m_forward; }
    const DirectX::XMVECTOR& getRight() const { return m_right; }

    void setPos(const DirectX::XMVECTOR& pos);
    void setPos(const DirectX::XMFLOAT3& pos);
    void setPos(float x, float y, float z);
    void move(const DirectX::XMVECTOR& pos);    //Adjusts to current position + input value
    void move(const DirectX::XMFLOAT3& pos);    //Adjusts to current position + input value
    void move(float x, float y, float z);       //Adjusts to current position + input value
    void setRotation(const DirectX::XMVECTOR& rot);
    void setRotation(const DirectX::XMFLOAT3& rot);
    void setRotation(float x, float y, float z);
    void rotate(const DirectX::XMVECTOR& rot);  //Ajdusts to current rotation + input value
    void rotate(const DirectX::XMFLOAT3& rot);  //Ajdusts to current rotation + input value
    void rotate(float x, float y, float z);     //Ajdusts to current rotation + input value

private:
    void updateWorld();
    
    Model m_model;

    DirectX::XMMATRIX m_world = DirectX::XMMatrixIdentity();

    DirectX::XMVECTOR m_posVec;
    DirectX::XMVECTOR m_RotationVec;
    DirectX::XMFLOAT3 m_pos;
    DirectX::XMFLOAT3 m_rotation;

    DirectX::XMVECTOR m_right;
    DirectX::XMVECTOR m_forward;

    const DirectX::XMVECTOR DEFAULT_FORWARD_VECTOR = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
    const DirectX::XMVECTOR DEFAULT_UP_VECTOR = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
    const DirectX::XMVECTOR DEFAULT_RIGHT_VECTOR = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);
};