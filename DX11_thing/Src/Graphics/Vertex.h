#pragma once
#include <DirectXMath.h>
#include "../FloatCompare.h"

struct Vertex
{
    Vertex(float x, float y, float z, float u, float v, float nX, float nY, float nZ) :
        pos(x, y, z), 
        tex(u, v),
        normal(nX, nY, nZ) {}

    Vertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT2 uv) :
        pos(pos),
        tex(uv),
        normal(normal) {}

    Vertex():
        pos(0, 0, 0),
        tex(0, 0),
        normal(0, 0, 0) {}

    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 tex;
};

//Could be optimised to return early
static bool operator==(const DirectX::XMFLOAT3& left, const DirectX::XMFLOAT3& right)
{
    return  FloatCompare::IsEqual(left.x, right.x, FLT_EPSILON) &&
            FloatCompare::IsEqual(left.y, right.y, FLT_EPSILON) &&
            FloatCompare::IsEqual(left.z, right.z, FLT_EPSILON);
}

static bool operator==(const DirectX::XMFLOAT2& left, const DirectX::XMFLOAT2& right)
{
    return  FloatCompare::IsEqual(left.x, right.x, FLT_EPSILON) &&
            FloatCompare::IsEqual(left.y, right.y, FLT_EPSILON);
}

static bool operator==(const Vertex& left, const Vertex& right)
{
    return left.normal == right.normal && left.pos == right.pos && left.tex == right.tex;
}