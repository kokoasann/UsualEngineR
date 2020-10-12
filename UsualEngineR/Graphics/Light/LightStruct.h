#pragma once



namespace UER
{
	struct SDirectionLight
	{
		Vector3 dir = { 0,-1,0 };
		Vector4 color = {1,1,1,1};
	};
	struct SPointLight
	{
		Vector3 pos = g_vec3Zero;
		Vector3 posInView = g_vec3Zero;
		Vector4 color = { 1,1,1,1 };
		float decay = 0.0;
		float radius = 10.f;
	};
}