#pragma once

#include <random>


namespace UER
{
	class GameRandom
	{
	public:
		static GameRandom& Get()
		{
			static GameRandom rand;
			return rand;
		}

		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="seed"></param>
		void Init(UINT seed);

		/// <summary>
		/// 0~1�̗�����Ԃ���
		/// </summary>
		/// <returns></returns>
		double Rand()
		{
			return double(m_engine())/double(m_max);
		}

		/// <summary>
		/// �����Ő��������ő�l�B
		/// </summary>
		/// <returns></returns>
		UINT Max() const
		{
			return m_max;
		}

		/// <summary>
		/// �����Ő��������ŏ��l�B
		/// </summary>
		/// <returns></returns>
		UINT Min() const
		{
			return m_min;
		}
	private:
		std::mt19937 m_engine;	// �����Z���k�!!²����������!!!
		UINT m_seed = 0;		// �V�[�h�n�B
		UINT m_min = 0;			// �ő�B
		UINT m_max = 0;			// �ŏ��B
	};
	/// <summary>
	/// ����������B(�����Z���k�c�C�X�^�[)
	/// </summary>
	/// <returns></returns>
	static GameRandom& GRandom()
	{
		return GameRandom().Get();
	}
}