#pragma once

//#include "ModelPS.fxh"
#include "ModelData.fxh"
#include "CameraData.fxh"
#include "PSInputStruct.fxh"
//
//  アニメーションしないモデル用のシェーダ
//

StructuredBuffer<float4x4> InstanceMatrix : register(t10);

// //モデル用の定数バッファ
// cbuffer ModelCb : register(b0){
// 	float4x4 mWorld;
// 	float4x4 mView;
// 	float4x4 mProj;
// };


//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal	: NORMAL;		//法線。
	float2 uv 		: TEXCOORD0;	//UV座標。
	float3 tangent	: TANGENT;		//接ベクトル
	float3 binormal : BINORMAL;
};



//頂点シェーダー。
SPSIn VSMain(SVSIn vsIn,uint id:SV_InstanceID)
{
	SPSIn psIn;
	psIn.pos = mul(InstanceMatrix[id], vsIn.pos);
	psIn.worldPos = psIn.pos;
	psIn.pos = mul(cam_mView, psIn.pos);
	psIn.pos = mul(cam_mProj, psIn.pos);
	psIn.normal = normalize(mul(InstanceMatrix[id], vsIn.normal));
	psIn.tangent = normalize(mul(InstanceMatrix[id], vsIn.tangent));
	psIn.binormal = normalize(mul(InstanceMatrix[id], vsIn.binormal));
	psIn.uv = vsIn.uv;

	return psIn;
}