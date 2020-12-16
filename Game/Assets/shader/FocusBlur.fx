#include "G_Sampler.fxh"

cbuffer BlurData:register(b0)
{
    int bd_samplingNum;
    float bd_samplingLen;
    float2 bd_center;
    float bd_decay;
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

float3 PSMain(PSInput IN):SV_Target0
{
    float3 res = 0.f;

    float centerLen = length(IN.toCenter);
    float2 toCenterVec = IN.toCenter*rcp(centerLen);

    for(int i=0;i<bd_samplingNum;i++)
    {
        res += g_tex.Sample(g_sampler,IN.uv+toCenterVec*(bd_samplingLen*i));
    }

    res *= rcp((float)bd_samplingNum);
    return res;
}