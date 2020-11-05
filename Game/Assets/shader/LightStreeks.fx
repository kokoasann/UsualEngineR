struct VSInput
{
	float4 pos:POSITION;
	float2 uv:TEXCOORD0;
};
struct PS_BlurInput 
{
	float4 pos	: SV_POSITION;
	float2 tex0	: TEXCOORD0;
	float2 tex1 : TEXCOORD1;
	float2 tex2 : TEXCOORD2;
	float2 tex3 : TEXCOORD3;
	float2 tex4 : TEXCOORD4;
	float2 tex5 : TEXCOORD5;
	float2 tex6 : TEXCOORD6;
	float2 tex7 : TEXCOORD7;
    float2 offset : TEXCOORD8;
};

cbuffer BlurParam:register(b0)
{
	//float4 offset;
	float4 weight[2];
    float2 streeksDir;
    float streeksLen;
}

Texture2D<float4> srcTex:register(t0);	//�ڂ����e�N�X�`��
sampler Sampler:register(s0);

/// <summary>
/// 
/// </summary>
PS_BlurInput VSMain(VSInput In)
{
	float2 texSize;
	float level;
	srcTex.GetDimensions(0, texSize.x, texSize.y, level);
	PS_BlurInput Out;
	Out.pos = In.pos;

	float2 tex = In.uv;
	float pixSize = -1.0f * rcp(texSize.x);
    float step = streeksLen * 0.125f;

	tex -= float2(pixSize*0.5f,0.f);

    float2 sps = streeksDir * pixSize * step;
    Out.offset = sps * 7.f;

	Out.tex0 = tex;// + streeksDir * pixSize*1.0f;
	Out.tex1 = mad(sps, 1.f, tex);
	Out.tex2 = mad(sps, 2.f, tex);
	Out.tex3 = mad(sps, 3.f, tex);
	Out.tex4 = mad(sps, 4.f, tex);
	Out.tex5 = mad(sps, 5.f, tex);
	Out.tex6 = mad(sps, 6.f, tex);
	Out.tex7 = tex + Out.offset;

	return Out;
}


/// <summary>
/// 
/// </summary>
float4 PSMain(PS_BlurInput In):SV_Target0
{
	float4 Color;
	Color  = weight[0].x * (srcTex.Sample(Sampler, In.tex0)
						+ srcTex.Sample(Sampler, In.tex7 - In.offset.xy));
	Color = mad(
        weight[0].y,
        srcTex.Sample(Sampler, In.tex1) + srcTex.Sample(Sampler, In.tex6 - In.offset.xy),
        Color);
	Color = mad(
        weight[0].z,
        srcTex.Sample(Sampler, In.tex2) + srcTex.Sample(Sampler, In.tex5 - In.offset.xy),
        Color);
	Color = mad(
        weight[0].w,
        srcTex.Sample(Sampler, In.tex3) + srcTex.Sample(Sampler, In.tex4 - In.offset.xy),
        Color);
	Color = mad(
        weight[1].x,
        srcTex.Sample(Sampler, In.tex4) + srcTex.Sample(Sampler, In.tex3 - In.offset.xy),
        Color);
	Color = mad(
        weight[1].y,
        srcTex.Sample(Sampler, In.tex5) + srcTex.Sample(Sampler, In.tex2 - In.offset.xy),
        Color);
	Color = mad(
        weight[1].z,
        srcTex.Sample(Sampler, In.tex6) + srcTex.Sample(Sampler, In.tex1 - In.offset.xy),
        Color);
	Color = mad(
        weight[1].w,
        srcTex.Sample(Sampler, In.tex7) + srcTex.Sample(Sampler, In.tex0 - In.offset.xy),
        Color);
	//return float4(0,0,1,1);
	return Color;
	//return float4(Color.xyz,1);
}