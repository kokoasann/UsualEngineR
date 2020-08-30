#include "PreCompile.h"
#include "UsualEngine.h"
#include "GraphicsEngine.h"


namespace UER
{
	
	UsualEngine* g_engine = nullptr;
	
	UsualEngine::~UsualEngine()
	{
		if (m_graphicsEngine) {
			delete m_graphicsEngine;
		}
	}
	void UsualEngine::Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
	{
		//グラフィックエンジンの初期化。
		m_graphicsEngine = new GraphicsEngine();
		m_graphicsEngine->Init(hwnd, frameBufferWidth, frameBufferHeight);
		//ゲームパッドの初期化。
		for (int i = 0; i < GamePad::CONNECT_PAD_MAX; i++) {
			g_pad[i] = &m_pad[i];
		}

		m_pGameObjectManeger = GameObjectManager::Get();
	}
	void UsualEngine::GameLoop()
	{
		// ここからゲームループ。
		while (DispatchWindowMessage())
		{
			Stopwatch sw;
			sw.Start();

			//レンダリング開始。
			g_engine->BeginFrame();


			/*#
			###	RayTracingの奴の残留思念。
			###
			if (isInit == false) {
				//g_graphicsEngine->RegistGeometoryToRaytracingEngine(model);
				//g_graphicsEngine->RegistGeometoryToRaytracingEngine(bgModel);
				//g_graphicsEngine->CommitRegistGeometoryToRaytracingEngine(renderContext);
				isInit = true;
			}
			else {

				//////////////////////////////////////
				//ここから絵を描くコードを記述する。
				//////////////////////////////////////
				//g_graphicsEngine->DispatchRaytracing(renderContext);
			}
			*/

			m_pGameObjectManeger->Update();


			//////////////////////////////////////
			//絵を描くコードを書くのはここまで！！！
			//////////////////////////////////////
			//レンダリング終了。
			g_engine->EndFrame();

			auto frameTime = sw.Stop();
			gameTime()->PushFrameDeltaTime(frameTime);
		}
	}
	void UsualEngine::BeginFrame()
	{
		m_graphicsEngine->BeginRender();
		for (auto& pad : m_pad) {
			pad.BeginFrame();
			pad.Update();
		}
		
	}
	void UsualEngine::EndFrame()
	{
		m_graphicsEngine->EndRender();
	}

}
