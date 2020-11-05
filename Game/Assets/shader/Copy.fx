/// <summary>
/// コピー
/// </summary>

#include "ScreenVertexProcess.fxh"

// struct VSInput {
// 	float4 pos : POSITION;
// 	float2 uv  : TEXCOORD0;
// };

// struct PSInput {
// 	float4 pos : SV_POSITION;
// 	float2 uv  : TEXCOORD0;
// };

struct PSOutput_2
{
	float4 col1:SV_Target0;
	float4 col2:SV_Target1;
};

struct PSOutput_3
{
	float4 col1:SV_Target0;
	float4 col2:SV_Target1;
	float4 col3:SV_Target2;
};

Texture2D<float4> sceneTexture_1 : register(t0);	//origin
Texture2D<float4> sceneTexture_2 : register(t1);	//origin
Texture2D<float4> sceneTexture_3 : register(t2);	//origin

sampler Sampler : register(s0);

// PSInput VSMain(VSInput In)
// {
// 	PSInput psIn;
// 	psIn.pos = In.pos;
// 	psIn.uv = In.uv;
// 	return psIn;
// }
float4 PSMain(PSInput In) : SV_Target0
{
	float4 res = sceneTexture_1.Sample(Sampler, In.uv);
	return res;
}

float4 PSMain_Double(PSInput In): SV_Target0
{
	float4 output;
	output = sceneTexture_1.Sample(Sampler, In.uv);
	output += sceneTexture_2.Sample(Sampler, In.uv);
	return output;
}

float4 PSMain_Triple(PSInput In): SV_Target0
{
	float4 output;
	output = sceneTexture_1.Sample(Sampler, In.uv);
	output += sceneTexture_2.Sample(Sampler, In.uv);
	output += sceneTexture_3.Sample(Sampler, In.uv);
	return output;
}


//
//	compute shader
//

RWTexture2D<float4> outTexture_1 : register(u0);

// cbuffer CopyConstBuffer:register(b0)
// {
// 	float4 whReciprocal;		//コピー先のテクスチャの幅高の逆数(w1,h1,w2,h2)
// }

[numthreads(4,4,1)]
void CSMain_thd4x4x1(uint3 id:SV_DispatchThreadID)
{
	outTexture_1[id.xy] = sceneTexture_1[id.xy];
}
