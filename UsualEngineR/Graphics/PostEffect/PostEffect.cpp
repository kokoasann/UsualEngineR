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
		//四角形プリミティブを初期化。
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
		m_primitive.Cteate(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4, sizeof(SSimpleVertex), vertices, 4, Primitive::it_2byte, indices);

		m_backBufferTexture[0].InitFromD3DResource(g_graphicsEngine->GetBuckBufferResource(0));
		m_backBufferTexture[1].InitFromD3DResource(g_graphicsEngine->GetBuckBufferResource(1));

		
		auto device = g_graphicsEngine->GetD3DDevice();
		auto comAllo = g_graphicsEngine->GetCommandAllocator();
		
		/*ID3D12GraphicsCommandList4* comList;
		device->CreateCommandList(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT, comAllo, nullptr, IID_PPV_ARGS(&comList)
		);
		m_rc4Bloom.Init(comList);*/

		m_bloom.Init(this);
	}
	void PostEffect::Render()
	{
		auto& rc = g_graphicsEngine->GetRenderContext();
		m_bloom.Render(rc);
		m_bloom.CombineRender(rc, g_graphicsEngine->GetCurrentRenderTarget());
	}
	Texture* PostEffect::GetCurrentBackBufferTexture()
	{
		return &m_backBufferTexture[g_graphicsEngine->GetBackBufferIndex()];
	}
}