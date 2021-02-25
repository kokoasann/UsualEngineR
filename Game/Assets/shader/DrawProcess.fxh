#include "ShadowRecieve.fxh"
#include "LightData.fxh"
#include "PBRFunc.fxh"


float3 DrawProcess(
    float3 albedo,      //アルベドカラー
    float depth,        //深度値。
    float3 normal,      //法線。
    float specular,     //スペキュラ
    float3 worldPos,    //ピクセルのワールド座標。
	float2 uv,			//uv
    float3 camPos       //カメラのワールド座標。
)
{
	//ランバート拡散反射
	float3 lig = 0;
	
	float3 toEye = normalize(camPos - worldPos);

	// first light(shadow light)
	{
		int ligNo = 0;
		//Disney
		float NdotL = saturate( dot( normal, lig_DirLights[ligNo].dir )*-1.f);
		
		float3 diffuse = NormalizedDisneyDiffuse
		(
			normal,
			lig_DirLights[ligNo].dir,
			toEye,
			specular
		//) * lig_DirLights[ligNo].color *(specular) * NdotL;
		) * lig_DirLights[ligNo].color * NdotL;

		//スペキュラ反射
		float3 spec = BRDF
		(
			lig_DirLights[ligNo].dir,
			toEye,
			normal
		) * lig_DirLights[ligNo].color * specular;

		lig += (diffuse + spec) ;
		lig *= 1.f - ShadowRecieve(worldPos, uv, depth);
		//lig += ShadowRecieve_Debug(worldPos, uv, depth);
	}
    
	for( int ligNo = 1; ligNo < lig_DLcount; ligNo++ ){
		//Disney
		float NdotL = saturate( dot( normal, lig_DirLights[ligNo].dir )*-1.f);
		
		float3 diffuse = NormalizedDisneyDiffuse
		(
			normal,
			lig_DirLights[ligNo].dir,
			toEye,
			specular
		//) * lig_DirLights[ligNo].color *(specular) * NdotL;
		) * lig_DirLights[ligNo].color * NdotL;

		//スペキュラ反射
		float3 spec = BRDF
		(
			lig_DirLights[ligNo].dir,
			toEye,
			normal
		) * lig_DirLights[ligNo].color * specular;

		lig += (diffuse + spec) ;
	}

	float3 res = albedo * rcp(PI) * lig;
    return res;
}