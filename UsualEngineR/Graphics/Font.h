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

	private:
		FontDrawType m_fontDrawType = FT_None;
	};
}