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
		/// NewGO���ꂽ���̂̃f�[�^�B
		/// </summary>
		struct NewGOData
		{
			GameObject* go = nullptr;	//New���ꂽ��̃|�C���^
			char prio = 0;				//�v���C�I���e�B
		};
		static GameObjectManager* Get()
		{
			static GameObjectManager* ins = nullptr;
			if (ins == nullptr)
				ins = new GameObjectManager();
			
			return ins;
		}

		/// <summary>
		/// �S�~���ɓ����Ă�I�u�W�F�N�g�����o���B
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
		/// �I�u�W�F�N�g�𐶐�����B
		/// </summary>
		/// <param name="prio">�v���C�I���e�B</param>
		/// <param name="name">���O</param>
		/// <param name="isCheckIned">�`�F�b�N�C�����Ă�H</param>
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
		/// �I�u�W�F�N�g������
		/// �I�u�W�F�N�g���g���܂킵�ł���悤�ɂȂ��Ă�����̂̓f���[�g�͂���Ȃ��B
		/// </summary>
		/// <param name="go">�����I�u�W�F�N�g</param>
		void DeleteGameObject(GameObject* go);

		/// <summary>
		/// �X�V�B
		/// </summary>
		void Update();
		/// <summary>
		/// Start���X�V
		/// </summary>
		void UpdateStart();
		/// <summary>
		/// Update���X�V
		/// </summary>
		void UpdateUpdate();
		/// <summary>
		/// PreRender���X�V
		/// </summary>
		void UpdatePreRender();
		/// <summary>
		/// Render���X�V
		/// </summary>
		void UpdateRender();
		/// <summary>
		/// PrePostRender���X�V�B
		/// </summary>
		void UpdatePrePostRender();
		/// <summary>
		/// PostRender���X�V
		/// </summary>
		void UpdatePostRender();

		void UpdateDeadProcess();

		/// <summary>
		/// �I�u�W�F�N�g���X�g�̃L���p�V�e�B��add�����₷
		/// </summary>
		/// <param name="prio">�v���C�I���e�B</param>
		/// <param name="add">���₷��</param>
		void AddReserved(int prio, int add);
		/// <summary>
		/// �I�u�W�F�N�g���X�g�̃L���p�V�e�B��Ԃ�
		/// </summary>
		/// <param name="prio">�v���C�I���e�B</param>
		/// <returns></returns>
		int Capacity(int prio);
		/// <summary>
		/// �I�u�W�F�N�g���X�g�̗]���������B
		/// CheckIN�̃J�E���g��0�ɂ���B
		/// </summary>
		/// <param name="prio">�v���C�I���e�B</param>
		void CapacityShrink2Fit(int prio);
		/// <summary>
		/// �`�F�b�N�C��
		/// </summary>
		/// <param name="prio">�v���C�I���e�B</param>
		/// <param name="count">�J�E���g</param>
		void CheckIN(int prio, int count);

		void AddThread(GameObject* obj, void(GameObject::* func)(void));
		void ReleaseThread();
	private:
		static const size_t m_maxSize = 32;					//priority�̍ő�l�B
		static std::mutex m_newGOPush_Mutex;
		std::array<std::vector<GameObject*>,m_maxSize> m_gameObjectList;	//�����Ă���Q�[���I�u�W�F�N�g�̃��X�g
		std::vector<NewGOData> m_newGOBuffer;				//���̃t���[���ɐ������ꂽ�I�u�W�F�N�g�̃��X�g
		
		std::array<int, m_maxSize> m_checkInCountList;		//
		std::vector<DeadData> m_ddList;						//DeleteGO���ꂽ�I�u�W�F�N�g�̃��X�g
		std::vector<GameObject*> m_trashBox;				//�ė��p�����I�u�W�F�N�g�̃��X�g

		bool m_isReleaseProcess = false;					//�S�I�u�W�F�N�g�̊J���������H

		struct ThreadState
		{
			ThreadObject* thread;

		};
		std::vector<ThreadObject*> m_threadList;
	};
	extern Lock<GameObjectManager> g_lockGameObjectManager;

	/// <summary>
	/// �I�u�W�F�N�g�𐶐�����
	/// </summary>
	/// <param name="prio">�v���C�I���e�B</param>
	/// <param name="name">���O</param>
	/// <param name="isTakeTrush">�g���܂킵����</param>
	/// <returns></returns>
	template<class T>
	static T* NewGO(int prio, const char* name="")
	{
		return GameObjectManager::Get()->NewGameObject<T>(prio, name, false,false);
	}

	/// <summary>
	/// �I�u�W�F�N�g�𐶐�����
	/// </summary>
	/// <param name="prio">�v���C�I���e�B</param>
	/// <param name="name">���O</param>
	/// <param name="isTakeTrush">�g���܂킵����</param>
	/// <returns></returns>
	template<class T>
	static T* NewGO(int prio, bool isTakeTrush)
	{
		return GameObjectManager::Get()->NewGameObject<T>(prio, "", isTakeTrush, false);
	}
	
	/// <summary>
	/// �I�u�W�F�N�g������
	/// �g���܂킵������z�̓f���[�g�����Ȃ�
	/// </summary>
	/// <param name="obj">�����I�u�W�F�N�g</param>
	template<class T>
	static void DeleteGO(T*& obj)
	{
		GameObjectManager::Get()->DeleteGameObject(obj);
		obj = nullptr;
	}


	/// <summary>
	/// num������GameObjectManager��TrushBox��Ɋm�ۂ���B
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