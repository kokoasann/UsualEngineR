
#include "ScreenVertexProcess.fxh"
#include "DrawProcess.fxh"
#include "GBufferData.fxh"
#include "Math.fxh"
#include "G_Sampler.fxh"

cbuffer DeferredCB:register(b0)
{
    float4x4 def_mvpi;
    float4 def_camPos;
    float4 def_camDir;
}

float4 PSMain(PSInput In):SV_Target0
{
    float3 albedo = g_GDiffuse.Sample(g_sampler,In.uv).rgb;
    float depth = g_GDepth.Sample(g_sampler,In.uv).r;
    float3 normal = g_GNormal.Sample(g_sampler,In.uv).rgb;
    float specular = g_GSpecular.Sample(g_sampler,In.uv).r;
    float3 worldPos = GetWorldPosition(In.uv,depth,def_mvpi);
    
    float4 res = float4(DrawProcess(albedo,depth,normal,specular,worldPos,def_camPos.xyz),1.f);
    return res;
}