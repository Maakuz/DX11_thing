cbuffer buf : register(b0)
{
    float4x4 mat;
};

struct VS_INPUT
{
    float3 pos : POS;
    float3 tex : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

VS_OUT main(VS_INPUT input)
{
    VS_OUT output;
    output.pos = mul(float4(input.pos, 1), mat);
    output.tex = input.tex;
    
    return output;
}