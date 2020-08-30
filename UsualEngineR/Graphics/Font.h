#pragma once


namespace UER
{
	class Font
	{
	public:
		Font();
		~Font();

		void Init();

		//�t�H���g�ɂ������
		enum FontDrawType
		{
			FT_None,
			FT_Frame,
			FT_Shadow,
		};
		/// <summary>
		/// �����t���Ȃ��B
		/// </summary>
		void DrawTypeNone()
		{
			m_fontDrawType = FT_None;
		}
		/// <summary>
		/// �g��t����B
		/// </summary>
		void DrawTypeFrame()
		{
			m_fontDrawType = FT_Frame;
		}
		/// <summary>
		/// �e��t����B(������)
		/// </summary>
		void DrawTypeShadow()
		{
			m_fontDrawType = FT_Shadow;
		}

		/// <summary>
		/// �J�n�B
		/// </summary>
		void Begin();
		/// <summary>
		/// �I���B
		/// </summary>
		void End();
		void Draw(const wchar_t* text, const Vector2& pos, const Vector4& color, float rot, float sca, const Vector2& pivot = { 0.5f,0.5f });

	private:
		FontDrawType m_fontDrawType = FT_None;
	};
}