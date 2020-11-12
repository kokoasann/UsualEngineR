cbuffer cb :register (b0)
{
	matrix mvp;			//ビュープロジェクション行列
	float4 mulcol;		//乗算色
}
struct VSInput {
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput {
	float4 pos  : SV_POSITION;
	float2 uv   : TEXCOORD0;
    uint id     : TEXCOORD1;
};

Texture2D<float4> colorTexture : register(t0);	//カラーテクスチャ。
sampler Sampler : register(s0);

struct ParticleData
{
	float4x4 mWorld;
	float4 mulColor;
};

StructuredBuffer<ParticleData> g_instanceData :register(t1);

PSInput VSMain(VSInput In,uint id:SV_InstanceID)
{
	PSInput psIn;
	psIn.pos = mul(g_instanceData[id].mWorld,In.pos);
	psIn.pos = mul(mvp,psIn.pos);
	psIn.uv = In.uv;
    psIn.id = id;
    return psIn;
}


float4 PSMain(PSInput In) : SV_Target0
{
	float4 res = colorTexture.Sample(Sampler, In.uv);// * mulcol;
    res *= mulcol * g_instanceData[In.id].mulColor;
	return res;
}


Texture2D<float1> g_GDepth : register(t2);

float4 PSMain_DepthEnable(PSInput In) : SV_Target0
{
    float rw = rcp(In.pos.w);
    float d = In.pos.z;// * rw;
    float2 screenPos = In.pos.xy;
	screenPos.x *= rcp(1280.f);
	screenPos.y *= rcp(720.f);
    float gd = g_GDepth.Sample(Sampler, screenPos);
    clip(gd-d);
	//return float4(1,1,0,1.f);

	float4 res = colorTexture.Sample(Sampler, In.uv);// * mulcol;
    res *= mulcol * g_instanceData[In.id].mulColor;
	return res;
}