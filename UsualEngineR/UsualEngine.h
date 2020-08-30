#pragma once

#include "HID/GamePad.h"


namespace UER
{
	
	class GraphicsEngine;
	
	class UsualEngine {
	public:
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~UsualEngine();
		/// <summary>
		/// フレームの開始時に呼ばれる処理。
		/// </summary>
		void BeginFrame();
		/// <summary>
		/// フレームの終了時に呼ばれる処理。
		/// </summary>
		void EndFrame();
		/// <summary>
		/// ゲームエンジンの初期化。
		/// </summary>
		void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);

		void GameLoop();
	private:
		GraphicsEngine* m_graphicsEngine = nullptr;		//グラフィックエンジン。
		GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//ゲームパッド。
		
	};
	
	extern UsualEngine* g_engine;	//TKエンジン。

}
