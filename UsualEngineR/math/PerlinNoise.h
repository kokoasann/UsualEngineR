#pragma once



namespace UER
{
	class PerlinNoise2D
	{
	private:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="v"></param>
		/// <returns></returns>
		Vector2 Random2D(const Vector2& v) const;
	public:
		static PerlinNoise2D& Get()
		{
			static PerlinNoise2D ins;
			return ins;
		}

		/// <summary>
		/// Vector2����p�[�����m�C�Y�𐶐����܂��B
		/// </summary>
		/// <param name="v"></param>
		/// <returns></returns>
		float GenerateNoise(const Vector2& v) const;

		/// <summary>
		/// Random2D�̂��߂̃p�����[�^���w�肵�܂��B
		/// ����ɂ���Ď����Ȃǂ��ς��܂��B
		/// </summary>
		/// <param name="dt_x"></param>
		/// <param name="dt_y"></param>
		/// <param name="scaling"></param>
		void SetParamater(const Vector2& dt_x, const Vector2& dt_y, float scaling)
		{
			m_dot_x = dt_x;
			m_dot_y = dt_y;
			m_scaling = scaling;
		}
	private:
		Vector2 m_dot_x = Vector2(349.7f, 761.3f);	// random2D�Ŏg���p�����[�^
		Vector2 m_dot_y = Vector2(277.1f, 409.5f);	// random2D�Ŏg���p�����[�^
		float m_scaling = 19990.623f;				// random2D�Ŏg���p�����[�^(���͉��̐��N����)
	};
	static PerlinNoise2D& GPerlinNoise()
	{
		return PerlinNoise2D::Get();
	}
}