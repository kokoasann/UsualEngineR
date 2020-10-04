#include "PreCompile.h"
#include "PostEffect.h"



namespace UER
{
	namespace {
		struct SSimpleVertex {
			Vector4 pos;
			Vector2 tex;
		};
	}
	void PostEffect::Release()
	{
	}
	void PostEffect::Init()
	{
		//�l�p�`�v���~�e�B�u���������B
		SSimpleVertex vertices[] =
		{
			{
				Vector4(-1.0f, -1.0f, 0.0f, 1.0f),
				Vector2(0.0f, 1.0f),
			},
			{
				Vector4(1.0f, -1.0f, 0.0f, 1.0f),
				Vector2(1.0f, 1.0f),
			},
			{
				Vector4(-1.0f, 1.0f, 0.0f, 1.0f),
				Vector2(0.0f, 0.0f)
			},
			{
				Vector4(1.0f, 1.0f, 0.0f, 1.0f),
				Vector2(1.0f, 0.0f)
			}

		};
		short indices[] = { 0,1,2,3 };
		m_primitive.Cteate(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 4, sizeof(SSimpleVertex), vertices, 4, Primitive::it_2byte, indices);


	}
	void PostEffect::Render()
	{
	}
}