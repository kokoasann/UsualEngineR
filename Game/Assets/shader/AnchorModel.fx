cbuffer SConstBufferData:register(b0)
{
    float4x4 mwvp;
}

struct VSInput
{
    float3 pos      :POSITION;
    float3 normal   :NORMAL;
};

struct PSInput
{
    float4 pos      :SV_POSITION;
    float4 normal   :NORMAL;
};

//RWBuffer<float2> g_vertPos:register(u0);
RWStructuredBuffer<float3> g_vertPos:register(u0);

PSInput VSMain(VSInput In,uint id:SV_VertexID)
{
    PSInput Out;
    Out.pos = mul(mwvp,float4(In.pos,1.f));
    Out.normal = normalize(mul(mwvp,float4(In.normal,1.f)));

    g_vertPos[id] = Out.pos.xyw;
    return Out;
}

PSInput VSMain_inv(VSInput In)
{
    PSInput Out;
    Out.pos = mul(mwvp,float4(In.pos,1.f));
    float3 normal_inv = In.normal;// * -1.f;
    Out.normal = normalize(mul(mwvp,float4(normal_inv,1.f)));
    return Out;
}


struct PSOutput
{
    float depth     :SV_TARGET0;
    uint stencil    :SV_TARGET1;
};
PSOutput PSMain(PSInput In)
{
    PSOutput Out;
    //Out.depth = In.pos.z * rcp(In.pos.w);
    Out.depth = In.pos.z;
    Out.stencil = 1;
    return Out;
}


float PSMain_inv(PSInput In):SV_TARGET0
{
    float d = In.pos.z;
    return d;
}