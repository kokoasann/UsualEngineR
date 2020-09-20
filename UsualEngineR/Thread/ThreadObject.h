#pragma once



namespace UER
{
	/// <summary>
	/// お手軽なthreadを提供致します。
	/// 面倒な開放とかその他いろいろをしなくて済みます!
	/// 基本的にメンバ変数にこいつを追加してExcute関数を呼ぶだけでOK
	/// </summary>
	class ThreadObject
	{
	public:
		ThreadObject();
		~ThreadObject();

		/// <summary>
		/// 開放。
		/// </summary>
		void Release();

		/// <summary>
		/// 実行。
		/// </summary>
		/// <param name="func">実行する関数/param>
		void Execute(std::function<void()> func);

		/// <summary>
		/// 終わった?
		/// </summary>
		/// <returns></returns>
		bool IsEnd() const
		{
			return m_isEnd;
		}
	private:
		std::thread* m_thread = nullptr;	//スレッド
		std::function<void()> m_func;		//スレッドで使う関数。

		bool m_isEnd = false;			//終わった?
	};
}