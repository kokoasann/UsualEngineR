/*!
 * @brief	プリミティブ用のシェーダー。
 */


#include "Light.fxh"
#include "PBRFunc.fxh"



//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};



//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal	: NORMAL;		//法線。
	float2 uv 		: TEXCOORD0;	//UV座標。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//法線。
	float2 uv 			: TEXCOORD0;	//uv座標。
	float3 worldPos		: TEXCOORD1;	//ワールド空間でのピクセルの座標。
};

//モデルテクスチャ。
Texture2D<float4> g_texture : register(t0);	
Texture2D<float4> g_normalMap : register(t1);
Texture2D<float4> g_specularMap : register(t2);

//サンプラステート。
sampler g_sampler : register(s0);


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

//テクスチャなしプリミティブ描画用のピクセルシェーダー。
float4 PSMain( SPSIn psIn ) : SV_Target0
{
	//法線を計算。
	float3 normal = psIn.normal;
	
	//ランバート拡散反射
	float3 lig = 0;
	
	float3 toEye = normalize(eyePos - psIn.worldPos);
	//メタリックは固定。
	float metaric = g_specularMap.Sample(g_sampler, psIn.uv).a;
	
	for( int ligNo = 0; ligNo < NUM_DIRECTIONAL_LIGHT; ligNo++ ){
		//Disney
		float NdotL = saturate( dot( normal, -directionalLight[ligNo].direction ));
		
		float3 diffuse = NormalizedDisneyDiffuse(normal, -directionalLight[ligNo].direction, toEye, 1.0f-metaric) * directionalLight[ligNo].color *( 1.0f-metaric) * NdotL;
		//スペキュラ反射
		float3 spec = BRDF(-directionalLight[ligNo].direction, toEye, normal) * directionalLight[ligNo].color * metaric;
		lig += (diffuse + spec) ;
	}
	//シンプルな環境光。
	lig += ambientLight;

	//アルベドカラー
	float4 albedoColor = g_texture.Sample(g_sampler, psIn.uv);
	float4 finalColor = 1.0f;
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}
