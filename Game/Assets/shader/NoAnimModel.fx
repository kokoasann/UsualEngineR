#include "ModelPS.fxh"

//
//  アニメーションしないモデル用のシェーダ
//


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
};



//頂点シェーダー。
SPSIn VSMain(SVSIn vsIn)
{
	SPSIn psIn;
	psIn.pos = mul(mWorld, vsIn.pos);
	psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	psIn.normal = normalize(mul(mWorld, vsIn.normal));
	
	psIn.uv = vsIn.uv;

	return psIn;
}