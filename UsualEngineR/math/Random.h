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
		/// 初期か。
		/// </summary>
		/// <param name="seed"></param>
		void Init(UINT seed);

		/// <summary>
		/// 0~1の乱数を返すよ
		/// </summary>
		/// <returns></returns>
		double Rand()
		{
			return double(m_engine())/double(m_max);
		}

		/// <summary>
		/// 乱数で生成される最大値。
		/// </summary>
		/// <returns></returns>
		UINT Max() const
		{
			return m_max;
		}

		/// <summary>
		/// 乱数で生成される最小値。
		/// </summary>
		/// <returns></returns>
		UINT Min() const
		{
			return m_min;
		}
	private:
		std::mt19937 m_engine;	// メルセンヌｯ!!ﾂｲｽﾀｧｱｱｱｧｱｧｧ!!!
		UINT m_seed = 0;		// シード地。
		UINT m_min = 0;			// 最大。
		UINT m_max = 0;			// 最小。
	};
	/// <summary>
	/// 乱数生成器。(メルセンヌツイスター)
	/// </summary>
	/// <returns></returns>
	static GameRandom& GRandom()
	{
		return GameRandom().Get();
	}
}