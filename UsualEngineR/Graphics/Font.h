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

		void DrawFrame(const wchar_t* text, const Vector2& pos, const Vector4& color, float rot, float sca, const Vector2& pivot);

		/// <summary>
		/// ������̕`�悵�����̃T�C�Y���o�́B
		/// </summary>
		/// <param name="str"></param>
		/// <returns></returns>
		Vector2 MeasureString(const wchar_t* str)
		{
			auto v = m_spriteFont->MeasureString(str);
			return Vector2(v.m128_f32[0], v.m128_f32[1]);
		}
	private:
		DirectX::SpriteBatch* m_spriteBatch = nullptr;		//�X�v���C�g�o�b�`
		DirectX::SpriteFont* m_spriteFont = nullptr;		//�t�H���g
		ID3D12DescriptorHeap* m_heap = nullptr;

		Matrix m_scaleMat = g_matIdentity;

		FontDrawType m_fontDrawType = FT_None;

		Vector4 m_fontEffectColor = Vector4::Vector4(0.0f, 0.0f, 0.0f, 1.0f);	//����̐F�B
		float m_fontEffectOffset = 1.0f;					//����̃I�t�Z�b�g
	};
}