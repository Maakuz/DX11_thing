#pragma once
#include <DirectXMath.h>

/*
Remember!!!

float2 [8] ->   [16]
float4 [16]->   [32]
float2 [8] ->   [48]
becomes 48 bytes

BUT 
float2 [8] ->   [16]
float2 [8] ->   [16]
float4 [16]->   [32]
Becomes 32 bytes since both float2 can fit in a 16 byte slot.

And also to pad the structs if neccessary
*/


namespace ConstantBuffers
{
    struct VS_vertexshader
    {
        DirectX::XMMATRIX mat;
    };

    struct PS_pixelshader
    {
        float alpha = 1;
    };
}