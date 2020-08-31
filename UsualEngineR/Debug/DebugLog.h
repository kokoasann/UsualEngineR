#pragma once


namespace UER
{
	class DebugLog
	{
	public:
		struct Log
		{
			std::wstring log;
			//const wchar_t* log = nullptr;
			float lifeTime = 0.0f;
		};
		DebugLog();
		~DebugLog();
		/// <summary>
		/// 左上にstrを表示するよ。
		/// </summary>
		/// <param name="str"></param>
		void Print(const wchar_t* str);
		/// <summary>
		/// 描画。
		/// </summary>
		void Render();
	private:
		Font m_font;												//フォント
		std::list<Log> m_printList;							//表示するログのリスト
		Vector2 m_originPos = { -640,360 };			//ログの表示位置。
		float m_lifeTime = 10.f;								//ログの生存時間。
		float m_firstFontSize = 0.7f;						//一番上に表示されるフォントのサイズ
		float m_firstFontSpace = 30.f;						//一番上に表示されるフォントの間隔。
		float m_fontSize = 0.4f;								//フォントのサイズ
		float m_fontSpace = 20.f;							//フォントの間隔。
		Vector4 m_fontColor = Vector4::White;	//フォントの色。
		int m_logMaxNum = 16;								//ログの表示される最大数。
	};
}