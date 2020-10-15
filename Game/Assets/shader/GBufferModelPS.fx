#pragma once

//#include "Light.fxh"
//#include "ModelData.fxh"
//#include "CameraData.fxh"
//#include "LightData.fxh"
//#include "PBRFunc.fxh"
#include "PSInputStruct.fxh"

struct SPSOut
{
    float4 albedo   :SV_TARGET0;
    float depth     :SV_TARGET1;
    float4 normal   :SV_TARGET2;
    float specular  :SV_TARGET3;
    float4 tangent  :SV_TARGET4;
};
//モデルテクスチャ。
Texture2D<float4> g_texture : register(t0);	
Texture2D<float4> g_normalMap : register(t1);
Texture2D<float4> g_specularMap : register(t2);

//サンプラステート。
sampler g_sampler : register(s0);

//テクスチャなしプリミティブ描画用のピクセルシェーダー。
SPSOut PSMain( SPSIn psIn )
{
    SPSOut Out;
	
    Out.albedo = g_texture.Sample(g_sampler, psIn.uv);
    Out.depth = psIn.pos.z * rcp(psIn.pos.w);
    Out.normal = float4(psIn.normal,1.f);
    Out.specular = 0.f;
    Out.tangent = float4(psIn.tangent,1.f);
    return Out;
}

SPSOut PSMain_nor( SPSIn psIn )
{
    SPSOut Out;
	
    Out.albedo = g_texture.Sample(g_sampler, psIn.uv);
    Out.depth = psIn.pos.z * rcp(psIn.pos.w);
    Out.normal = g_normalMap.Sample(g_sampler, psIn.uv);
    Out.specular = 0.f;
    Out.tangent = float4(psIn.tangent,1.f);
    return Out;
}

SPSOut PSMain_spe( SPSIn psIn )
{
    SPSOut Out;
	
    Out.albedo = g_texture.Sample(g_sampler, psIn.uv);
    Out.depth = psIn.pos.z * rcp(psIn.pos.w);
    Out.normal = float4(psIn.normal,1.f);
    Out.specular = g_specularMap.Sample(g_sampler, psIn.uv).r;
    Out.tangent = float4(psIn.tangent,1.f);
    return Out;
}

SPSOut PSMain_nor_spe( SPSIn psIn )
{
    SPSOut Out;
	
    Out.albedo = g_texture.Sample(g_sampler, psIn.uv);
    Out.depth = psIn.pos.z * rcp(psIn.pos.w);
    Out.normal = g_normalMap.Sample(g_sampler, psIn.uv);
    Out.specular = g_specularMap.Sample(g_sampler, psIn.uv).r;
    Out.tangent = float4(psIn.tangent,1.f);
    return Out;
}