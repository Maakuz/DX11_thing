cbuffer buf : register(b0)
{
    float xOffset;
    float yOffset;
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
    input.pos.x += xOffset;
    input.pos.y += yOffset;
    
    output.pos = float4(input.pos, 1.0f);
    output.tex = input.tex;
    
    return output;
}