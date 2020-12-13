
cbuffer LineData:register(b0)
{
	float4x4 mView;
	float4x4 mProj;
}

struct VSIn
{
	float3 pos:POSITION;
	float3 color:COLOR;
};
struct PSIn
{
	float4 pos:SV_POSITION;
	float3 color:COLOR;
};

PSIn VSMain(VSIn In)
{
	PSIn outpos;
	outpos.pos = mul(mView,float4(In.pos,1.f));
	outpos.pos = mul(mProj, outpos.pos);
	outpos.color = In.color;
	
	return outpos;
}

float4 PSMain(PSIn In) :SV_TARGET0
{
	return float4(In.color,1.f);
}