#pragma once


namespace UER
{


	/// <summary>
	/// マルチスレッドをする際に変数を排他制御するためのクラス
	/// 
	/// コンストラクタの引数で制御したいインスタンスを渡すか、
	/// Set関数で引数で制御したいインスタンスを渡せば準備完了。
	/// </summary>
	template<class T>
	class Lock
	{
	public:
		/// <summary>
		/// ロックされてる変数を内包するクラス
		/// </summary>
		struct Content
		{
			friend Lock;	//ContentのものはLockのもの!

			/// <summary>
			/// コピーコンストラクタ
			/// </summary>
			/// <param name="c"></param>
			Content(const Content& c)
			{
				*this = c;
				lock->m_isUsing = true;
			}
			
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="c">内容物のポインタ</param>
			/// <param name="lock">管理者</param>
			Content(T* c,Lock* lock)
			{
				this->lock = lock;
				content = c;
				lock->m_isUsing = true;
			}

			virtual ~Content()
			{
				lock->m_isUsing = false;
			}


			T* operator->()
			{
				return content;
			}
			
			T& Get()
			{
				return *content;
			}
		private:
			T* content = 0;	//内容物のポインタ。
			Lock* lock;	//管理者。
		};
		
		
		Lock() = delete;
		
		Lock(const Lock<T>& t) = default;
		Lock(T& content)
		{
			Set(&content);
		}
		Lock(T* content)
		{
			Set(content);
		}
		~Lock() {};


		/// <summary>
		/// 制御している変数を返す
		/// </summary>
		/// <returns></returns>
		Content Get()
		{
			while (m_isUsing)
			{
				Sleep(SLEEP_TIME);
			}
			m_isUsing = true;
			return m_contens;
		}
		/// <summary>
		/// 読み取り専用。
		/// </summary>
		/// <returns></returns>
		const T& Get_ReadOnly() const
		{
			return *m_content;
		}

		/// <summary>
		/// 内容物をセット
		/// </summary>
		/// <param name="t"></param>
		void Set(T* t)
		{
			m_content = t;
			m_contens.content = t;
		}
	private:
		
	private:
		static const int SLEEP_TIME = 10;			//ゲットの時にロック状態の場合にフラグの再調査をするまでの時間。
		Content m_contens = Content(nullptr,this);	//内容物
		T* m_content;	//内容物のポインタ
		
		bool m_isUsing = false;	//使われてる?
	};
}