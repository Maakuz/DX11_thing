#pragma once
#include <DirectXMath.h>

struct Vertex
{
    Vertex() :
        pos(0, 0, 0),
        tex(0, 0) {}

    Vertex(float x, float y, float z, float u, float v) :
        pos(x, y, z), 
        tex(u, v) {}

    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 tex;
};