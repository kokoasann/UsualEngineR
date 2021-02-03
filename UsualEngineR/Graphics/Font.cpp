#include "PreCompile.h"
#include "Font.h"


namespace UER
{
	Font::Font()
	{
		m_spriteBatch = g_graphicsEngine->GetSpriteBatch();
		m_spriteFont = g_graphicsEngine->GetSpriteFont();
		m_heap = g_graphicsEngine->GetFontDescriptorHeap();
		D3D12_VIEWPORT vp = { 0.f,0.f,FRAME_BUFFER_W ,FRAME_BUFFER_H,D3D12_MIN_DEPTH,D3D12_MAX_DEPTH };
		m_spriteBatch->SetViewport(vp);
	}
	Font::~Font()
	{
	}
	void Font::Init()
	{
		
	}
	void Font::Begin()
	{
		auto* cl = g_graphicsEngine->GetCommandList();
		ID3D12DescriptorHeap* heaps[] = { m_heap };
		cl->SetDescriptorHeaps(1, heaps);
		m_spriteBatch->Begin(g_graphicsEngine->GetCommandList(), DirectX::SpriteSortMode_Deferred, m_scaleMat);
	}
	void Font::End()
	{
		m_spriteBatch->End();
	}
	void Font::Draw(const wchar_t* text, const Vector2& pos, const Vector4& color, float rot, float sca, const Vector2& pivot)
	{
		m_spriteFont->MeasureString()
		Vector2 piv = pivot;
		piv.y = 1.0f - piv.y;
		Vector2 position;
		position.x = pos.x + FRAME_BUFFER_W * 0.5f;
		position.y = -pos.y + FRAME_BUFFER_H * 0.5f;

		switch (m_fontDrawType)
		{
		case FT_Frame:
			DrawFrame(text, position, m_fontEffectColor, rot, sca, piv);
			break;
		case FT_Shadow:
			//DrawShadow(text, position, m_fontEffectColor, rot, sca, piv);
			break;
		}

		m_spriteFont->DrawString(m_spriteBatch, text, position.vec, color, rot, DirectX::XMFLOAT2(piv.x, piv.y), sca, DirectX::SpriteEffects_None);
	}
	void Font::DrawFrame(const wchar_t* text, const Vector2& pos, const Vector4& color, float rot, float sca, const Vector2& pivot)
	{
		Vector2 ofsList[] = {
		{m_fontEffectOffset,0.f},
		{-m_fontEffectOffset,0.f},
		{0.f,m_fontEffectOffset},
		{0.f,-m_fontEffectOffset},
		{m_fontEffectOffset,m_fontEffectOffset},
		{m_fontEffectOffset,-m_fontEffectOffset},
		{-m_fontEffectOffset,-m_fontEffectOffset},
		{-m_fontEffectOffset,m_fontEffectOffset},
		};
		for (const auto& ofs : ofsList)
		{
			m_spriteFont->DrawString(m_spriteBatch, text, (pos + ofs).vec, color, rot, DirectX::XMFLOAT2(pivot.x, pivot.y), sca, DirectX::SpriteEffects_None);
		}
	}
}