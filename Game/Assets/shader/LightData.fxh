#pragma once

/*//////////////////////////////////////////////////////////////////////////////////////
	ライト系.
*//////////////////////////////////////////////////////////////////////////////////////

struct SDirectionLight
{
	float3 dir;			//ディレクション
	float4 color;		//色
};

StructuredBuffer<SDirectionLight> lig_DirLights : register(t10);

struct SPointLight
{
	float3 pos;
	float3 posInView;
	float4 color;
	float decay;
	float radius;
};

StructuredBuffer<SPointLight> lig_PntLights : register(t11);

cbuffer LightCB : register(b2)
{
	float4 lig_screenSize : packoffset(c0);		//スクリーンのサイズ
	//float3 lig_eyepos : packoffset(c1);			//視点
	int lig_DLcount : packoffset(c1.w);			//ディレクションライトの数
	int lig_PLcount : packoffset(c2.x);			//ポイントライトの数
}