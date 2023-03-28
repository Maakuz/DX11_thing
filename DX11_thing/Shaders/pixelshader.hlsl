cbuffer alpha : register(b0)
{
    float alpha;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D tex : TEXTURE : register(t0);
Texture2D tex2 : TEXTURE : register(t1);
SamplerState samplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 col = tex.Sample(samplerState, input.tex.xy);
    return float4(col, alpha);
}