#pragma once

//#include "Light.fxh"
#include "ModelData.fxh"
#include "CameraData.fxh"
#include "LightData.fxh"
#include "PBRFunc.fxh"
#include "PSInputStruct.fxh"

//ピクセルシェーダーへの入力。
// struct SPSIn{
// 	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
// 	float3 normal		: NORMAL;		//法線。
// 	float2 uv 			: TEXCOORD0;	//uv座標。
// 	float3 worldPos		: TEXCOORD1;	//ワールド空間でのピクセルの座標。
// };

//モデルテクスチャ。
Texture2D<float4> g_texture : register(t0);	
Texture2D<float4> g_normalMap : register(t1);
Texture2D<float4> g_specularMap : register(t2);

//サンプラステート。
sampler g_sampler : register(s0);

//テクスチャなしプリミティブ描画用のピクセルシェーダー。
float4 PSMain( SPSIn psIn ) : SV_Target0
{
	//return float4(0,0,0,1);
	//法線を計算。
	float3 normal = psIn.normal;
	
	//ランバート拡散反射
	float3 lig = 0;
	
    //float3 eyePos = {0,0,0};
	float3 eyePos = cam_Pos;
	float3 toEye = normalize(eyePos - psIn.worldPos);
	//メタリックは固定。
	float metaric;
	//float metaric = g_specularMap.Sample(g_sampler, psIn.uv).a;
	metaric = 0;
	#if 1
	for( int ligNo = 0; ligNo < lig_DLcount; ligNo++ ){
		//Disney
		float NdotL = saturate( dot( normal, lig_DirLights[ligNo].dir ));
		
		float3 diffuse = NormalizedDisneyDiffuse
		(
			normal,
			lig_DirLights[ligNo].dir,
			toEye,
			1.0f-metaric
		) * lig_DirLights[ligNo].color *( 1.0f-metaric) * NdotL;

		//スペキュラ反射
		float3 spec = BRDF
		(
			lig_DirLights[ligNo].dir,
			toEye,
			normal
		) * lig_DirLights[ligNo].color * metaric;

		lig += (diffuse + spec) ;
	}
	#endif

	//シンプルな環境光。
	//lig += ambientLight;

	//アルベドカラー
	float4 albedoColor = g_texture.Sample(g_sampler, psIn.uv);
	float4 finalColor = 1.0f;
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}
