

static const float PI = 3.14159265358979323846;

float Beckmann(float m, float t)
{
    float M = m*m;
    float T = t*t;
    return exp((T-1)*rcp(M*T)) * rcp(M*T*T);
}

float spcFresnel(float f0, float u)
{
    // from Schlick
    //return f0 + (1-f0) * pow(1-u, 5);
    return mad((1-f0), pow(1-u, 5), f0);
}

float BRDF( float3 L, float3 V, float3 N )
{
	float microfacet = 1.0f;
	float f0 = 0.5;
	bool include_F = 0;
	bool include_G = 0;
    // compute the half float3
    float3 H = normalize( L + V );

    float NdotH = dot(N, H);
    float VdotH = dot(V, H);
    float NdotL = dot(N, L);
    float NdotV = dot(N, V);
 	
    float D = Beckmann(microfacet, NdotH);
    float F = spcFresnel(f0, VdotH);
	
	float t = 2.0 * NdotH * rcp(VdotH);
    //float G = max( 0.0f, min(1.0, min(t * NdotV, t * NdotL)) );
    float G = saturate(min(t * NdotV, t * NdotL));

	float m = 3.14159265 * NdotV * NdotL;
	/*
    NdotH = NdotH + NdotH;
    float G = (NdotV < NdotL) ? 
        ((NdotV*NdotH < VdotH) ?
         NdotH / VdotH :
         oneOverNdotV)
        :
        ((NdotL*NdotH < VdotH) ?
         NdotH*NdotL / (VdotH*NdotV) :
         oneOverNdotV);
	*/
    //if (include_G) G = oneOverNdotV;
    return max(F * D * G * rcp(m), 0.0);
}

float SchlickFresnel(float u, float f0, float f90)
{
  //return f0 + (f90-f0)*pow(1.0f-u,5.0f);
  return mad((f90-f0),pow(1.0f-u,5.0f),f0);
}

float3 NormalizedDisneyDiffuse(float3 N, float3 L, float3 V, float roughness)
{
  //光源に向かうベクトルと視線に向かうベクトルのハーフベクトルを求める。
  float3 H = normalize(L+V);

  float energyBias = lerp(0.0f, 0.5f, roughness);
  //float energyFactor = lerp(1.0f, 1.0f/1.51f, roughness);
  #define RCP_1_51 0.6622516556291390728476821192053
  float energyFactor = lerp(1.0f, RCP_1_51, roughness);
  //光源に向かうベクトルとハーフベクトルがどれだけ似ているかを内積で求める。
  float dotLH = saturate(dot(L,H));
  //法線と光源に向かうベクトルがどれだけ似ているかを内積で求める。
  float dotNL = saturate(dot(N,L));
  //法線と視線に向かうベクトルがどれだけ似ているかを内積で求める。
  float dotNV = saturate(dot(N,V));

  //float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;
  float Fd90 = mad(2.0, dotLH * dotLH * roughness,energyBias);
  
  float FL = SchlickFresnel(1.0f, Fd90, dotNL);
  float FV = SchlickFresnel(1.0f, Fd90, dotNV);
  return (FL*FV)*rcp(PI);
}