struct VS_INPUT
{
    float3 pos : POS;
    float3 col : COL;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float3 col : COL;
};

VS_OUT main(VS_INPUT input)
{
    VS_OUT output;
    output.pos = float4(input.pos, 1.0f);
    output.col = input.col;
    
    return output;
}