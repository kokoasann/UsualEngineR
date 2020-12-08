#pragma once

#include "GameObject.h"
#include "util/Util.h"

namespace UER
{
	struct DeadData
	{
		int prio = 0;
		int ind = 0;
	};
	class GameObjectManager
	{
	private:
		GameObjectManager();
		
	public:
		
		~GameObjectManager();
		void Release();

		/// <summary>
		/// NewGOされたもののデータ。
		/// </summary>
		struct NewGOData
		{
			GameObject* go = nullptr;	//Newされたやつのポインタ
			char prio = 0;				//プライオリティ
		};
		static GameObjectManager* Get()
		{
			static GameObjectManager* ins = nullptr;
			if (ins == nullptr)
				ins = new GameObjectManager();
			
			return ins;
		}

		/// <summary>
		/// ゴミ箱に入ってるオブジェクトを取り出す。
		/// </summary>
		/// <returns></returns>
		template<class T>
		T* TakeTrash()
		{
			T* t = NULL;
			for (GameObject* tr : m_trashBox)
			{
				t = dynamic_cast<T*>(tr);
				if (t != NULL)
				{
					break;
				}
			}
			if (t != NULL)
			{
				auto it = std::find(m_trashBox.begin(), m_trashBox.end(), t);
				m_trashBox.erase(it);
				//ob = (GameObject*)(t);
				t->Revive();
			}
			return t;
		}

		/// <summary>
		/// オブジェクトを生成する。
		/// </summary>
		/// <param name="prio">プライオリティ</param>
		/// <param name="name">名前</param>
		/// <param name="isCheckIned">チェックインしてる？</param>
		/// <returns></returns>
		template<class T>
		T* NewGameObject(int prio, const char* name,bool isTakeTrush,bool isCheckIned)
		{
			if (prio >= m_maxSize)
			{
				//MessageBoxA(usualEngine()->GetHwnd(),"","",0)
				assert(0);
			}
			int hash = Util::MakeHash(name);

			T* go = TakeTrash<T>();
			if (go == nullptr)
			{
				go = new T();
			}

			GameObject* o = (GameObject*)go;
			o->Awake();
			o->SetPrio(prio);
			o->SetName(hash);
			o->CreatInGameObjectManager();
			if (isTakeTrush)
				o->TrashTaking();
			//m_gameObjectList[prio].push_back((GameObject*)go);

			m_newGOPush_Mutex.lock();

			m_newGOBuffer.push_back({ go,(char)prio });
			if (isCheckIned && m_checkInCountList[prio]>0)
				m_checkInCountList[prio] -= 1;

			m_newGOPush_Mutex.unlock();

			return go;
		}

		/// <summary>
		/// オブジェクトを消す
		/// オブジェクトが使いまわしできるようになっているものはデリートはされない。
		/// </summary>
		/// <param name="go">消すオブジェクト</param>
		void DeleteGameObject(GameObject* go);

		/// <summary>
		/// 更新。
		/// </summary>
		void Update();
		/// <summary>
		/// Startを更新
		/// </summary>
		void UpdateStart();
		/// <summary>
		/// Updateを更新
		/// </summary>
		void UpdateUpdate();
		/// <summary>
		/// PreRenderを更新
		/// </summary>
		void UpdatePreRender();
		/// <summary>
		/// Renderを更新
		/// </summary>
		void UpdateRender();
		/// <summary>
		/// PrePostRenderを更新。
		/// </summary>
		void UpdatePrePostRender();
		/// <summary>
		/// PostRenderを更新
		/// </summary>
		void UpdatePostRender();

		void UpdateDeadProcess();

		/// <summary>
		/// オブジェクトリストのキャパシティをadd分増やす
		/// </summary>
		/// <param name="prio">プライオリティ</param>
		/// <param name="add">増やす数</param>
		void AddReserved(int prio, int add);
		/// <summary>
		/// オブジェクトリストのキャパシティを返す
		/// </summary>
		/// <param name="prio">プライオリティ</param>
		/// <returns></returns>
		int Capacity(int prio);
		/// <summary>
		/// オブジェクトリストの余分を消す。
		/// CheckINのカウントを0にする。
		/// </summary>
		/// <param name="prio">プライオリティ</param>
		void CapacityShrink2Fit(int prio);
		/// <summary>
		/// チェックイン
		/// </summary>
		/// <param name="prio">プライオリティ</param>
		/// <param name="count">カウント</param>
		void CheckIN(int prio, int count);

		void AddThread(GameObject* obj, void(GameObject::* func)(void));
		void ReleaseThread();
	private:
		static const size_t m_maxSize = 32;					//priorityの最大値。
		static std::mutex m_newGOPush_Mutex;
		std::array<std::vector<GameObject*>,m_maxSize> m_gameObjectList;	//生きているゲームオブジェクトのリスト
		std::vector<NewGOData> m_newGOBuffer;				//そのフレームに生成されたオブジェクトのリスト
		
		std::array<int, m_maxSize> m_checkInCountList;		//
		std::vector<DeadData> m_ddList;						//DeleteGOされたオブジェクトのリスト
		std::vector<GameObject*> m_trashBox;				//再利用されるオブジェクトのリスト

		bool m_isReleaseProcess = false;					//全オブジェクトの開放処理中？

		struct ThreadState
		{
			ThreadObject* thread;

		};
		std::vector<ThreadObject*> m_threadList;
	};
	extern Lock<GameObjectManager> g_lockGameObjectManager;

	/// <summary>
	/// オブジェクトを生成する
	/// </summary>
	/// <param name="prio">プライオリティ</param>
	/// <param name="name">名前</param>
	/// <param name="isTakeTrush">使いまわしする</param>
	/// <returns></returns>
	template<class T>
	static T* NewGO(int prio, const char* name="")
	{
		return GameObjectManager::Get()->NewGameObject<T>(prio, name, false,false);
	}

	/// <summary>
	/// オブジェクトを生成する
	/// </summary>
	/// <param name="prio">プライオリティ</param>
	/// <param name="name">名前</param>
	/// <param name="isTakeTrush">使いまわしする</param>
	/// <returns></returns>
	template<class T>
	static T* NewGO(int prio, bool isTakeTrush)
	{
		return GameObjectManager::Get()->NewGameObject<T>(prio, "", isTakeTrush, false);
	}
	
	/// <summary>
	/// オブジェクトを消す
	/// 使いまわしをする奴はデリートをしない
	/// </summary>
	/// <param name="obj">消すオブジェクト</param>
	template<class T>
	static void DeleteGO(T*& obj)
	{
		GameObjectManager::Get()->DeleteGameObject(obj);
		obj = nullptr;
	}


	/// <summary>
	/// num分だけGameObjectManagerのTrushBox上に確保する。
	/// </summary>
	/// <param name="num"></param>
	/// <param name="prio"></param>
	template<class T>
	static void AllocateGO(int num,int prio)
	{
		for (int i = 0; i < num; i++)
		{
			auto go = NewGO<T>(prio, true);
			DeleteGO(go);
		}
	}

	template<class T>
	static void AllocateGO(int num, int prio, std::vector<T*>& goList)
	{
		goList.reserve(goList.size() + num);
		for (int i = 0; i < num; i++)
		{
			auto go = NewGO<T>(prio, true);
			goList.push_back(go);
			DeleteGO(go);
		}
	}
}