#pragma once

namespace UER
{
	class GameObject
	{
	public:
		enum ThreadingFunction
		{
			tf_None = 0,

			tf_Awake = 1,
			tf_Start = 2,
			
			tf_Update = 4,
			tf_PreUpdate = 8,
			tf_PostUpdate = 16,

			tf_Render = 32,
			tf_PreRender = 64,
			tf_PostRender = 128,
		};

		GameObject();
		virtual ~GameObject();

		void Revive()
		{
			m_isStartGO = false;
			m_isDeadGO = false;
			m_isActiveGO = true;
			m_nameGO = 0;
			m_prioGO = 0;
			m_isTrashTakeGO = false;
		}

		virtual void Release() = 0;

		//�f�X�g���N��
		virtual void OnDestroy() =0;
		//���߂̃R���X�g���N�^
		virtual void Awake() {};
		//�X�^�[�g�֐��@�߂�l��false�̏ꍇ�͎��̃t���[���ł��������Ă΂��
		virtual bool Start() { return true; };

		//�X�^�[�g�֐���true���Ԃ��Ă������̃t���[�����疈�t���[���Ă΂��悤�ɂȂ�B Update�̑O�ɌĂ΂��
		virtual void PreUpdate() {};
		//�X�^�[�g�֐���true���Ԃ��Ă������̃t���[�����疈�t���[���Ă΂��悤�ɂȂ�B
		virtual void Update() {};
		//�X�^�[�g�֐���true���Ԃ��Ă������̃t���[�����疈�t���[���Ă΂��悤�ɂȂ�BUpdate�̌�ɌĂ΂��B
		virtual void PostUpdate() {};

		virtual void Render() {};
		virtual void PostRender() {};

		void WrapOnDestroy(){}
		void WrapStart();
		void WrapPreUpdate();
		void WrapUpdate();
		void WrapPostUpdate();
		void WrapRender();
		void WrapPostRender();

		/*
		�Q�[�����ł͎g��Ȃ�
		FindGO�Ƃ��Ŏ��ʂ��邽�߂̖��O���Z�b�g����
		*/
		void SetName(int name)
		{
			m_nameGO = name;
		}
		/*
		�Q�[�����ł͎g��Ȃ�
		�����̗D��x�B�����Ⴂ�قǐ�ɌĂ΂��B
		*/
		void SetPrio(int prio)
		{
			m_prioGO = prio;
		}
		//�D��x��Ԃ�
		int GetPrio() const
		{
			return m_prioGO;
		}

		//�X�^�[�g�֐���true���Ԃ��Ă������B
		bool IsStart() const
		{
			return m_isStartGO;
		}
		//�X�^�[�g����
		void DoneStart()
		{
			m_isStartGO = true;
		}

		//����ł�H
		bool IsDead() const
		{
			return m_isDeadGO;
		}
		//����
		void DoneDead()
		{
			m_isDeadGO = true;
		}

		//�A�N�e�B�u�H
		bool IsActive() const
		{
			return m_isActiveGO;
		}
		//�A�N�e�B�u�t���O���Z�b�g����
		void SetActive(bool b)
		{
			m_isActiveGO = b;
		}

		//�S�~�E��������
		void TrashTaking()
		{
			m_isTrashTakeGO = true;
		}
		//�S�~�E������H
		bool IsTrashTake() const
		{
			return m_isTrashTakeGO;
		}

		void CreatInGameObjectManager()
		{
			m_isCreatedInGOM = true;
		}
		bool IsCreatedInGameObjedtManager() const
		{
			return m_isCreatedInGOM;
		}

		void SetThreadingFunction(UINT flag)
		{
			m_threadingFunctionFlag = flag;
		}
		void AddThreadingFunciton(UINT flag)
		{
			m_threadingFunctionFlag |= flag;
		}
		UINT GetThreadingFunction() const
		{
			return m_threadingFunctionFlag;
		}

	private:
		bool m_isStartGO = false;			//�X�^�[�g�֐��Ă΂ꂽ�H
		bool m_isDeadGO = false;			//���񂾁H
		bool m_isActiveGO = true;			//������H

		int m_nameGO = 0;					//���O(hash�l)
		int m_prioGO = 0;						//�v���C�I���e�B

		bool m_isTrashTakeGO = false;	//�S�~�E���H
		bool m_isCreatedInGOM = false; //�Q�[���I�u�W�F�N�g�}�l�[�W���[�ō쐬���ꂽ�B

		UINT m_threadingFunctionFlag = tf_None;
	protected:
	};

}

