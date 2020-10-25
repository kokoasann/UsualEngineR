#include "Math.fxh"

cbuffer SConstBufferData
{
    float4x4 cb_mvp;
    float4x4 cb_mvp_inv;
}

struct VSInput
{
    float4 pos  :POSITION;
    float2 uv   :TEXCOORD;
};

struct PSInput
{
    float4 pos  :SV_POSITION;
    float2 uv   :TEXCOORD;

};


PSInput VSMain(VSInput In)
{
    PSInput Out;

    Out.pos = In.pos;
    Out.uv = In.uv;

    return Out;
}

Texture2D<float> g_anchor : register(t0);   	//
Texture2D<float> g_anchor_inv : register(t1);	//
Texture2D<uint> g_stencil : register(t2);	    //
Texture2D<float> g_GDepth : register(t3);	    //

sampler Sampler : register(s0);

float4 PSMain(PSInput In):SV_TARGET0
{
    float frontDepth = g_anchor.Sample(Sampler,In.uv).x;
    float backDepth = g_anchor_inv.Sample(Sampler,In.uv).x;
    float gDepth = g_GDepth.Sample(Sampler,In.uv).x;
    uint2 iuv = In.uv * uint2(1280,720);
    float stencil = g_stencil[iuv];
    
    clip(min(gDepth-frontDepth,stencil-0.1f));

    float3 pos = GetWorldPosition(In.uv,frontDepth,cb_mvp_inv);
    float3 backPos = GetWorldPosition(In.uv,backDepth,cb_mvp_inv);
    //float3 gPos = GetWorldPosition(In.uv,gDepth,cb_mvp_inv);
    float3 dir = normalize(backPos - pos);
    

    float vol = 0.f;

    #define ray_count 80
    [unroll(ray_count)]
    for(int i=0;i<ray_count;i++)
    {
        vol += 0.01f;

        pos += dir*2.f;

        float4 vpPos = mul(cb_mvp,float4(pos,1.f));
        float vpdep = vpPos.z * rcp(vpPos.w);
        if(backDepth < vpdep || gDepth < vpdep)
        {
            break;
        }
#if 0
        //ちょっとコストが高い?ので、上の処理を使う。
        float3 r2b = normalize(backPos - pos)*1.2;
        float r2blen = length(r2b-dir);

        float3 r2g = normalize(gPos - pos)*1.2;
        float r2glen = length(r2g-dir);

        if(r2blen >= 2.f || r2glen >= 2.f)
        {
            break;
        }
#endif
    }
    vol = saturate(vol);
    float4 res = float4(1,1,1,vol);
    
    return res;
}