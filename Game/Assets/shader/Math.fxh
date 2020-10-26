

//uvと深度値とビュープロジェクション逆行列からワールド座標を求める。
float3 GetWorldPosition(float2 uv,float depth,float4x4 mVPI)
{
    //float4 projPos = float4(uv*float2(2.0f,-2.0f)+float2(-1.0f,1.0f),depth,1.0f);
    float4 projPos = float4(mad(uv, float2(2.0f,-2.0f), float2(-1.0f,1.0f)),depth,1.0f);
    float4 pos = mul(mVPI,projPos);
    return pos.xyz * rcp(pos.w);
}