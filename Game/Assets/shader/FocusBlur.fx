#include "G_Sampler.fxh"
#include "PerlinNoise.fxh"
#include "FBM.fxh"

cbuffer BlurData:register(b0)
{
    int bd_samplingNum;
    float bd_samplingLen;
    float2 bd_center;
    float bd_decay;
    float bd_time;
}
Texture2D<float3> g_tex:register(t0);

struct VSInput {
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput {
	float4 pos  : SV_POSITION;
	float2 uv   : TEXCOORD0;
    float2 toCenter:TEXCOORD1;
};

PSInput VSMain(VSInput IN)
{
    PSInput OUT;
    OUT.pos = IN.pos;
    OUT.uv = IN.uv;
    OUT.toCenter = bd_center - IN.uv;
    return OUT;
}

float Random3D(in float3 v)
{
    return frac(sin(dot(v,float3(0.702,0.133,0.881)))*43758.5453123);
}
float4 PSMain(PSInput IN):SV_Target0
{
    float3 res = 0.f;

    float centerLen = length(IN.toCenter);
    float2 toCenterVec = IN.toCenter*rcp(centerLen);

    
    float samplingLen = bd_samplingLen * FBM3D_x3(float3(toCenterVec,bd_time*10.f)*10.f);
    for(int i=0;i<bd_samplingNum;i++)
    {
        res += g_tex.Sample(g_sampler,IN.uv+toCenterVec*(samplingLen*i));
    }

    res *= rcp((float)bd_samplingNum);
    float uvLen = length(IN.uv) / 0.707107;
    float a = saturate(length(abs(IN.uv - 0.5)*2.f));
    return float4(res,a);
}