

cbuffer cb :register (b0)
{
	matrix mvp;			//�r���[�v���W�F�N�V�����s��
	float4 mulcol;		//��Z�F
	float4 screenParam;
}
struct VSInput {
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> colorTexture : register(t0);	//�J���[�e�N�X�`���B
sampler Sampler : register(s0);

PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = mul(mvp,In.pos);
	psIn.uv = In.uv;
	return psIn;
}
float4 PSMain(PSInput In) : SV_Target0
{
	float4 res = colorTexture.Sample(Sampler, In.uv) * mulcol;
	return res;
}

