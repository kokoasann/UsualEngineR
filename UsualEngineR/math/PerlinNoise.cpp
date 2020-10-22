#include "PreCompile.h"
#include "PerlinNoise.h"



namespace UER
{

	Vector2 PerlinNoise2D::Random2D(const Vector2& v) const
	{

		Vector2 ve(v.Dot(m_dot_x),
			v.Dot(m_dot_y));
		
		ve.x = sin(ve.x);
		ve.y = sin(ve.y);
		float x = ve.x * m_scaling;
		float y = ve.y * m_scaling;
		ve.x = (x - (int)x)*2.f;
		ve.y = (y - (int)y)*2.f;

		ve.x -= 1.f;
		ve.y -= 1.f;

		return ve;
	}
	float PerlinNoise2D::GenerateNoise(const Vector2& v) const
	{
		Vector2 i((int)v.x,(int)v.y);
		Vector2 f(v.x - i.x, v.y - i.y);

		//float2 u = f * f * (3.f - 2.f * f);
		Vector2 u(
			f.x * f.x * (f.x * -2.f + 3.f),
			f.y * f.y * (f.y * -2.f + 3.f)
		);

		Vector2 ofs_xy(1, 0);
		Vector2 ofs_yx(0, 1);
		Vector2 ofs_yy(1, 1);

		float mixX1 = Math::Lerp(u.x, Random2D(i).Dot(f), Random2D(i + ofs_xy).Dot(f- ofs_xy));

		/*float mixX1 = lerp(dot(PerlinRandom2D(i + float2(0.f, 0.f)), f - float2(0.f, 0.f)),
			dot(PerlinRandom2D(i + float2(1.f, 0.f)), f - float2(1.f, 0.f)), u.x);*/

		float mixX2 = Math::Lerp(u.x, Random2D(i+ofs_yx).Dot(f-ofs_yx), Random2D(i + ofs_yy).Dot(f - ofs_yy));

		/*float mixX2 = lerp(dot(PerlinRandom2D(i + float2(0.f, 1.f)), f - float2(0.f, 1.f)),
			dot(PerlinRandom2D(i + float2(1.f, 1.f)), f - float2(1.f, 1.f)), u.x);*/
		return Math::Lerp(u.y, mixX1, mixX2);
	}
}