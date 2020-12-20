#pragma once

#include "HID/GamePad.h"
#include "GameObject/GameObjectManager.h"
#include "Sound/SoundEngine.h"

namespace UER
{
	class PhysicsWorld;
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


		PhysicsWorld& GetPhysics()
		{
			return *m_physicsWorld;
		}

		CSoundEngine& GetSoundEngine()
		{
			return m_soundEngine;
		}
	private:
		GraphicsEngine* m_graphicsEngine = nullptr;		//グラフィックエンジン。
		GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//ゲームパッド。
		GameObjectManager* m_pGameObjectManeger = nullptr;			//ゲームオブジェクトマネージャー
		PhysicsWorld* m_physicsWorld;
		CSoundEngine m_soundEngine;
	};
	
	
	extern UsualEngine* g_engine;	//TKエンジン。


	inline PhysicsWorld& Physics()
	{
		return g_engine->GetPhysics();
	}
	inline CSoundEngine& SoundEngine()
	{
		return g_engine->GetSoundEngine();
	}
}
