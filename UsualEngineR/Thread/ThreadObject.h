#pragma once



namespace UER
{
	/// <summary>
	/// お手軽なthreadを提供致します。
	/// 面倒な開放とかその他いろいろをしなくて済みます!
	/// 基本的にメンバ変数にこいつを追加してExcute関数を呼ぶだけでOK
	/// 
	/// 米米米 このクラスは使いまわしできません!! ただしRelease関数を呼ぶと使いまわせます。
	/// </summary>
	class ThreadObject
	{
	public:
		ThreadObject();
		~ThreadObject();

		/// <summary>
		/// 開放。
		/// 使いまわしたい場合も呼びましょう。
		/// </summary>
		void Release();

		/// <summary>
		/// 実行。
		/// </summary>
		/// <param name="func">実行する関数/param>
		void Execute(const std::function<void()>& func);

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

		bool m_isEnd = false;				//終わった?

		bool m_isExecuted = false;			//実行した?
	};
}