#pragma once
#include "LightData.fxh"
#include "PBRFunc.fxh"

float3 DrawProcess(
    float3 albedo,      //アルベドカラー
    float depth,        //深度値。
    float3 normal,      //法線。
    float specular,     //スペキュラ
    float3 worldPos,    //ピクセルのワールド座標。
    float3 camPos       //カメラのワールド座標。
)
{
	//ランバート拡散反射
	float3 lig = 0;
	
	float3 toEye = normalize(camPos - worldPos);
    
	for( int ligNo = 0; ligNo < lig_DLcount; ligNo++ ){
		//Disney
		float NdotL = saturate( dot( normal, lig_DirLights[ligNo].dir ));
		
		float3 diffuse = NormalizedDisneyDiffuse
		(
			normal,
			lig_DirLights[ligNo].dir,
			toEye,
			1.0f-specular
		) * lig_DirLights[ligNo].color *( 1.0f-specular) * NdotL;

		//スペキュラ反射
		float3 spec = BRDF
		(
			lig_DirLights[ligNo].dir,
			toEye,
			normal
		) * lig_DirLights[ligNo].color * specular;

		lig += (diffuse + spec) ;
	}

	float3 res = albedo * lig;
    return res;
}