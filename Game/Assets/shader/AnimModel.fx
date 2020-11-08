#pragma once

//#include "ModelPS.fxh"
#include "ModelData.fxh"
#include "CameraData.fxh"
#include "PSInputStruct.fxh"
//
//  アニメーションするモデル用のシェーダ
//

//ボーンマトリクスの配列。
StructuredBuffer<float4x4> boneMatrix : register(t5);

// //モデル用の定数バッファ
// cbuffer ModelCb : register(b0){
// 	float4x4 mWorld;
// 	float4x4 mView;
// 	float4x4 mProj;
// };


struct VSInputNmTxWeights
{
	float4 pos : POSITION;		    //頂点の座標。
	float3 normal : NORMAL;			    //法線。
	float2 uv : TEXCOORD0;		        //UV
	float3 tangent : TANGENT;			//tangent
	float3 binormal : BINORMAL;
	uint4  indices : BLENDINDICES0;		//ボーンの番号。
	float4 weights : BLENDWEIGHT0;		//ボーンのウェイト
};



//頂点シェーダー。
SPSIn VSMain(VSInputNmTxWeights vsIn)
{
	SPSIn psIn;

    float4x4 skin = 0;
    {
        float w=0;
        [unroll(3)]
        for(int i=0;i<3;i++)
        {
            skin += boneMatrix[vsIn.indices[i]] * vsIn.weights[i];
            w += vsIn.weights[i];
        }
        skin += boneMatrix[vsIn.indices[i]]*(1.0f-w);

        psIn.pos = mul(skin,vsIn.pos);
    }

	//psIn.pos = mul(mWorld, psIn.pos);
	psIn.worldPos = psIn.pos;
	psIn.pos = mul(cam_mView, psIn.pos);
	psIn.pos = mul(cam_mProj, psIn.pos);
	psIn.normal = normalize(mul(skin, vsIn.normal));
	psIn.tangent = normalize(mul(skin, vsIn.tangent));
	psIn.binormal = normalize(mul(skin, vsIn.binormal));
	psIn.uv = vsIn.uv;

	return psIn;
}