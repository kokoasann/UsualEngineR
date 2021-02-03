#pragma once


namespace UER
{
	class Font
	{
	public:
		Font();
		~Font();

		void Init();

		//フォントにつける飾り
		enum FontDrawType
		{
			FT_None,
			FT_Frame,
			FT_Shadow,
		};
		/// <summary>
		/// 飾りを付けない。
		/// </summary>
		void DrawTypeNone()
		{
			m_fontDrawType = FT_None;
		}
		/// <summary>
		/// 枠を付ける。
		/// </summary>
		void DrawTypeFrame()
		{
			m_fontDrawType = FT_Frame;
		}
		/// <summary>
		/// 影を付ける。(未実装)
		/// </summary>
		void DrawTypeShadow()
		{
			m_fontDrawType = FT_Shadow;
		}

		/// <summary>
		/// 開始。
		/// </summary>
		void Begin();
		/// <summary>
		/// 終了。
		/// </summary>
		void End();
		void Draw(const wchar_t* text, const Vector2& pos, const Vector4& color, float rot, float sca, const Vector2& pivot = { 0.5f,0.5f });

		void DrawFrame(const wchar_t* text, const Vector2& pos, const Vector4& color, float rot, float sca, const Vector2& pivot);

		/// <summary>
		/// 文字列の描画した時のサイズを出力。
		/// </summary>
		/// <param name="str"></param>
		/// <returns></returns>
		Vector2 MeasureString(const wchar_t* str)
		{
			auto v = m_spriteFont->MeasureString(str);
			return Vector2(v.m128_f32[0], v.m128_f32[1]);
		}
	private:
		DirectX::SpriteBatch* m_spriteBatch = nullptr;		//スプライトバッチ
		DirectX::SpriteFont* m_spriteFont = nullptr;		//フォント
		ID3D12DescriptorHeap* m_heap = nullptr;

		Matrix m_scaleMat = g_matIdentity;

		FontDrawType m_fontDrawType = FT_None;

		Vector4 m_fontEffectColor = Vector4::Vector4(0.0f, 0.0f, 0.0f, 1.0f);	//飾りの色。
		float m_fontEffectOffset = 1.0f;					//飾りのオフセット
	};
}