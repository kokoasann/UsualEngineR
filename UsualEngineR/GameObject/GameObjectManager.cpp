#include "PreCompile.h"
#include "GameObjectManager.h"

#include "util/Util.h"

#include "Debug/Debug.h"

namespace UER
{
	Lock<GameObjectManager> g_lockGameObjectManager = Lock<GameObjectManager>(nullptr);
	std::mutex GameObjectManager::m_newGOPush_Mutex;

	GameObjectManager::GameObjectManager()
	{
		m_newGOBuffer.reserve(16);
		g_lockGameObjectManager.Set(this);
	}

	GameObjectManager::~GameObjectManager()
	{
		Release();
	}

	void GameObjectManager::Release()
	{
		m_isReleaseProcess = true;

		for (auto& golist : m_gameObjectList)
		{
			for (auto go : golist)
			{
				if (go->IsCreatedInGameObjedtManager())
				{
					go->Release();
					delete go;
				}
			}
			golist.clear();
		}
		
		for (auto go : m_trashBox)
		{
			go->Release();
			delete go;
		}
		m_trashBox.clear();
	}

	void GameObjectManager::Update()
	{
		UpdateStart();
		//Stopwatch sw;
		//sw.Start();
		UpdateUpdate();
		/*auto t = sw.Stop();
		char str[255] = { 0 };
		sprintf(str, "%.3f\n", t);
		OutputDebugString(str);*/

		UpdateRender();
		UpdatePreRender();
		
		

		//auto& pe = usualEngine()->GetGraphicsEngine()->GetPostEffect();
		//pe.Render();

		UpdatePostRender();

		g_graphicsEngine->RenderPostEffect();

		Debug::Instance().DrawLog();

		for (auto& ngd : m_newGOBuffer)
		{
			m_gameObjectList[ngd.prio].push_back(ngd.go);
		}
		m_newGOBuffer.clear();
	}

	void GameObjectManager::UpdateStart()
	{
		for (auto& goList : m_gameObjectList)
		{
			for (auto go : goList)
			{
				if (go->GetThreadingFunction() & GameObject::tf_Start)
				{
					AddThread(go, &GameObject::WrapStart);
				}
				else
				{
					go->WrapStart();
				}


				if (!go->IsStart() && !go->IsDead())
				{
				}
			}
		}

		ReleaseThread();
		
	}

	void GameObjectManager::DeleteGameObject(GameObject* go)
	{
		if (go == nullptr || m_isReleaseProcess)
			return;
		go->OnDestroy();
		go->DoneDead();
	}

	void GameObjectManager::UpdateUpdate()
	{
		int Count = 0;
		for (auto& goList : m_gameObjectList)
		{
			for (auto go : goList)
			{
				if (go->GetThreadingFunction() & GameObject::tf_PreUpdate)
				{
					AddThread(go, &GameObject::WrapPreUpdate);
				}
				else
				{
					go->WrapPreUpdate();
				}
			}
		}

		ReleaseThread();


		for (auto& goList : m_gameObjectList)
		{
			for (auto go : goList)
			{
				if (go->GetThreadingFunction() & GameObject::tf_Update)
				{
					AddThread(go, &GameObject::WrapUpdate);
				}
				else
				{
					go->WrapUpdate();
				}
				
			}
		}

		ReleaseThread();


		for (auto& goList : m_gameObjectList)
		{
			for (auto go : goList)
			{
				if (go->GetThreadingFunction() & GameObject::tf_PostUpdate)
				{
					AddThread(go, &GameObject::WrapPostUpdate);
				}
				else
				{
					go->WrapPostUpdate();
				}
			}
		}

		ReleaseThread();

		
		//Ž€‚ñ‚¾“z‚Ìˆ—B
		for (auto& goList : m_gameObjectList)
		{
			for (auto go : goList)
			{
				if (go->IsDead())
				{
					DeadData dd;
					dd.ind = Count;
					dd.prio = go->GetPrio();
					m_ddList.push_back(dd);
				}
				Count++;
			}
			Count = 0;
		}
		Count = 0;
		int nowprio = 0;
		for (auto& dd : m_ddList)
		{
			if (nowprio != dd.prio)
			{
				Count = 0;
				nowprio = dd.prio;
			}
			GameObject* go = m_gameObjectList[dd.prio][dd.ind-Count];
			auto it = std::find(m_gameObjectList[dd.prio].begin(), m_gameObjectList[dd.prio].end(), go);
			m_gameObjectList[dd.prio].erase(it);
			if (go->IsCreatedInGameObjedtManager())
			{
				if (go->IsTrashTake())
					m_trashBox.push_back(go);
				else
					delete go;
			}
			Count++;

		}
		m_ddList.clear();
	}

	void GameObjectManager::UpdatePreRender()
	{
		//usualEngine()->GetGraphicsEngine()->PreRenderDraw();
	}

	void GameObjectManager::UpdateRender()
	{
		for (auto& goList : m_gameObjectList)
		{
			for (auto go : goList)
			{
				go->Render();
			}
		}
	}

	void GameObjectManager::UpdatePostRender()
	{
		for (auto& goList : m_gameObjectList)
		{
			for (auto go : goList)
			{
				go->PostRender();
			}
		}
	}

	void GameObjectManager::AddReserved(int prio, int add)
	{
		m_gameObjectList[prio].reserve(m_gameObjectList[prio].capacity() + add);
	}
	int GameObjectManager::Capacity(int prio)
	{
		int cap = m_gameObjectList[prio].capacity();
		cap -= m_checkInCountList[prio];
		return cap;
	}
	void GameObjectManager::CapacityShrink2Fit(int prio)
	{
		m_checkInCountList[prio] = 0;
		m_gameObjectList[prio].shrink_to_fit();
	}
	void GameObjectManager::CheckIN(int prio, int count)
	{
		m_checkInCountList[prio] += count;
	}


	void GameObjectManager::AddThread(GameObject* obj, void(GameObject::* func)(void))
	{
		ThreadObject* thread = nullptr;
		for (auto th : m_threadList)
		{
			if (th->IsEnd())
			{
				th->Release();
				thread = th;
				break;
			}
		}

		if (thread == nullptr)
		{
			thread = new ThreadObject();
			m_threadList.push_back(thread);
		}

		thread->Execute([obj,func]()
			{
				(obj->*func)();
			});
	}

	void GameObjectManager::ReleaseThread()
	{
		for (auto th : m_threadList)
		{
			th->Release();
		}
	}
}