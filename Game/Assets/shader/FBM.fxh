#define FBM_COUNT_5 5
#define FBM_COUNT_3 3


float FBMRandom2D(in float2 v)
{
    return frac(sin(dot(v,float2(12.9898,78.233)))*43758.5453123);
}

float FBMNoise2D (in float2 st) {
    float2 i = floor(st);
    float2 f = frac(st);

    // Four corners in 2D of a tile
    float a = FBMRandom2D(i);
    float b = FBMRandom2D(i + float2(1.0, 0.0));
    float c = FBMRandom2D(i + float2(0.0, 1.0));
    float d = FBMRandom2D(i + float2(1.0, 1.0));

    //float2 u = f * f * (3.0 - 2.0 * f);
    float2 u = f * f * mad(f, -2.0f, 3.0f);
    
    return mad((d - b) * u.x, u.y, mad((c - a) * u.y, (1.0 - u.x), lerp(a, b, u.x)));
    // return lerp(a, b, u.x) +
    //         (c - a)* u.y * (1.0 - u.x) +
    //         (d - b) * u.x * u.y;
}

//非整数ブラウン運動(2D)ループ5回。
float FBM2D_x5(float2 pos)
{
    float res = 0;
    float amplitude = 0.5f;
    [unroll(FBM_COUNT_5)]
    for(int i=0;i < FBM_COUNT_5;i++)
    {
        res += amplitude * FBMNoise2D(pos);
        pos *= 2.;
        amplitude *= 0.5;
    }
    return res;
}

//非整数ブラウン運動(2D)ループ3回。
float FBM2D_x3(float2 pos)
{
    float res = 0;
    float amplitude = 0.5f;
    [unroll(FBM_COUNT_3)]
    for(int i=0;i < FBM_COUNT_3;i++)
    {
        res += amplitude * FBMNoise2D(pos);
        pos *= 2.;
        amplitude *= 0.5;
    }
    return res;
}


float FBMRandom3D(in float3 v)
{
    return frac(sin(dot(v,float3(0.702,0.133,0.881)))*43758.5453123);
}
//周期が短い
float FBMRandom3D_(in float3 p)  // replace this by something better
{
    p  = 50.*frac( p*2.702 + float3(0.005,0.004,0.004));
    return -1.0+2.0*frac( p.x*p.y*p.z*(p.x+p.y+p.z) );
}


float FBMNoise3D (in float3 st) {
    float3 i = floor(st);
    float3 f = frac(st);

    float p000 = FBMRandom3D(i);
    float p100 = FBMRandom3D(i + float3(1.0, 0.0, 0.0));
    float p010 = FBMRandom3D(i + float3(0.0, 1.0, 0.0));
    float p110 = FBMRandom3D(i + float3(1.0, 1.0, 0.0));
    float p001 = FBMRandom3D(i + float3(0.0, 0.0, 1.0));
    float p101 = FBMRandom3D(i + float3(1.0, 0.0, 1.0));
    float p011 = FBMRandom3D(i + float3(0.0, 1.0, 1.0));
    float p111 = FBMRandom3D(i + float3(1.0, 1.0, 1.0));

    float3 u = f*f*f*mad(f,mad(f,6.f,-15.f),10.f);
    
    float k0 =   p000;
    float k1 =   p100 - p000;
    float k2 =   p010 - p000;
    float k3 =   p001 - p000;
    float k4 =   p000 - p100 - p010 + p110;
    float k5 =   p000 - p010 - p001 + p011;
    float k6 =   p000 - p100 - p001 + p101;
    float k7 = - p000 + p100 + p010 - p110 + p001 - p101 - p011 + p111;
    
    // float w = (
    //     k0 +
    //     k1 * u.x +
    //     k2 * u.y +
    //     k3 * u.z +
    //     k4 * u.x * u.y +
    //     k5 * u.y * u.z +
    //     k6 * u.z * u.x +
    //     k7 * u.x * u.y * u.z
    // );
    float w = mad(k7,u.x * u.y * u.z,
    mad(k6,u.z * u.x,
    mad(k5,u.y * u.z,
    mad(k4,u.x * u.y,
    mad(k3,u.z,
    mad(k2,u.y,
    mad(k1,u.x,k0)))))));
    
    return w;
    //return (w+1.)*0.5;
}

//非整数ブラウン運動(3D)
float FBM3D_x5(float3 pos,in float3 shift)
{
    float res = 0;
    float amplitude = 0.5f;
    [unroll(FBM_COUNT_5)]
    for(int i=0;i < FBM_COUNT_5;i++)
    {
        res += amplitude * FBMNoise3D(pos);
        pos = pos * 2. + shift;
        amplitude *= 0.5;
    }
    return res;
}

//非整数ブラウン運動(3D)
float FBM3D_x3(float3 pos,in float3 shift)
{
    float res = 0;
    float amplitude = 0.5f;
    [unroll(FBM_COUNT_3)]
    for(int i=0;i < FBM_COUNT_3;i++)
    {
        res += amplitude * FBMNoise3D(pos);
        pos = pos * 2. + shift;
        amplitude *= 0.5;
    }
    return res;
}


float FBM3D_FBMx2(float3 pos,float3 move,float3 offsetScale)
{
    float res = 0;
    return res;
}