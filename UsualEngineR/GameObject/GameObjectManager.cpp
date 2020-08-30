#include "PreCompile.h"
#include "GameObjectManager.h"

#include "util/Util.h"

#include "Debug/Debug.h"

namespace UER
{
	GameObjectManager::GameObjectManager()
	{
		m_newGOBuffer.reserve(16);
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
		}

		for (auto go : m_trashBox)
		{
			go->Release();
			delete go;
		}
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

		//UpdateRender();
		UpdatePreRender();
		


		//auto& pe = usualEngine()->GetGraphicsEngine()->GetPostEffect();
		//pe.Render();

		UpdatePostRender();

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
				if (!go->IsStart() && !go->IsDead())
				{
					if (go->Start())
						go->DoneStart();
				}
			}
		}
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
				go->WrapPreUpdate();
			}
		}
		for (auto& goList : m_gameObjectList)
		{
			for (auto go : goList)
			{
				go->WrapUpdate();
			}
		}
		for (auto& goList : m_gameObjectList)
		{
			for (auto go : goList)
			{
				go->WrapPostUpdate();
			}
		}
		
		//���񂾓z�̏����B
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
}