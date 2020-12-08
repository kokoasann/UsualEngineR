
#ifndef GBUFFER_DATA
#define GBUFFER_DATA

Texture2D<float3> g_GDiffuse    : register(t0);
Texture2D<float1> g_GDepth      : register(t1);
Texture2D<float3> g_GNormal     : register(t2);
Texture2D<float1> g_GSpecular   : register(t3);
Texture2D<float3> g_GTangent    : register(t4);
#endif