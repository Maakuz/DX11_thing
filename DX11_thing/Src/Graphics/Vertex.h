#pragma once
#include <DirectXMath.h>

struct Vertex
{
    Vertex() :
        pos(0, 0),
        col(0, 0, 0) {}

    Vertex(float x, float y, float r, float g, float b) :
        pos(x, y), 
        col(r, g, b) {}

    DirectX::XMFLOAT2 pos;
    DirectX::XMFLOAT3 col;
};