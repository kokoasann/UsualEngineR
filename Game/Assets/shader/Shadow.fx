

cbuffer ConstBuffer:register(b0)
{
    float4x4 mWorld;
    float4 mulcol;
}

cbuffer ConstBufferLight:register(b1)
{
    float4x4 mvp;
}

//頂点シェーダーへの入力。
struct SVSIn
{
	float4 pos 		: POSITION;		//モデルの頂点座標。
    float3 normal : NORMAL;			    //法線。
	float2 uv : TEXCOORD0;		        //UV
	float3 tangent : TANGENT;			//tangent
	float3 binormal : BINORMAL;
	uint4  indices : BLENDINDICES0;		//ボーンの番号。
	float4 weights : BLENDWEIGHT0;		//ボーンのウェイト
};
struct SVSIn_Skin
{
	float4 pos 		: POSITION;		//モデルの頂点座標。
    float3 normal : NORMAL;			    //法線。
	float2 uv : TEXCOORD0;		        //UV
	float3 tangent : TANGENT;			//tangent
	float3 binormal : BINORMAL;
	uint4  indices : BLENDINDICES0;		//ボーンの番号。
	float4 weights : BLENDWEIGHT0;		//ボーンのウェイト
};

//ボーンマトリクスの配列。
StructuredBuffer<float4x4> boneMatrix : register(t5);

struct SPSIn
{
    float4 pos      :SV_Position;
};

SPSIn VSMain_NonSkin(SVSIn In)
{
    SPSIn psIn;
	psIn.pos = mul(mWorld, In.pos);
    psIn.pos = mul(mvp, psIn.pos);

    return psIn;
}

SPSIn VSMain_Skin(SVSIn_Skin vsIn)
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
	//psIn.pos = psIn.pos;
    psIn.pos = mul(mvp, psIn.pos);

    return psIn;
}

float PSMain(SPSIn In):SV_Target0
{
    float4 c = In.pos.z;
    c.a = 1.f;
    return In.pos.z;// * rcp(In.pos.w);
}