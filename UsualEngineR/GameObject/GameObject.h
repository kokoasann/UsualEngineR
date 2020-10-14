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

		//デストラクラ
		virtual void OnDestroy() =0;
		//早めのコンストラクタ
		virtual void Awake() {};
		//スタート関数　戻り値がfalseの場合は次のフレームでもっかい呼ばれる
		virtual bool Start() { return true; };

		//スタート関数でtrueが返ってきた次のフレームから毎フレーム呼ばれるようになる。 Updateの前に呼ばれる
		virtual void PreUpdate() {};
		//スタート関数でtrueが返ってきた次のフレームから毎フレーム呼ばれるようになる。
		virtual void Update() {};
		//スタート関数でtrueが返ってきた次のフレームから毎フレーム呼ばれるようになる。Updateの後に呼ばれる。
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
		ゲーム側では使わない
		FindGOとかで識別するための名前をセットする
		*/
		void SetName(int name)
		{
			m_nameGO = name;
		}
		/*
		ゲーム側では使わない
		処理の優先度。数が低いほど先に呼ばれる。
		*/
		void SetPrio(int prio)
		{
			m_prioGO = prio;
		}
		//優先度を返す
		int GetPrio() const
		{
			return m_prioGO;
		}

		//スタート関数でtrueが返ってきたか。
		bool IsStart() const
		{
			return m_isStartGO;
		}
		//スタートした
		void DoneStart()
		{
			m_isStartGO = true;
		}

		//死んでる？
		bool IsDead() const
		{
			return m_isDeadGO;
		}
		//死んだ
		void DoneDead()
		{
			m_isDeadGO = true;
		}

		//アクティブ？
		bool IsActive() const
		{
			return m_isActiveGO;
		}
		//アクティブフラグをセットする
		void SetActive(bool b)
		{
			m_isActiveGO = b;
		}

		//ゴミ拾いをする
		void TrashTaking()
		{
			m_isTrashTakeGO = true;
		}
		//ゴミ拾いする？
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
		bool m_isStartGO = false;			//スタート関数呼ばれた？
		bool m_isDeadGO = false;			//死んだ？
		bool m_isActiveGO = true;			//動ける？

		int m_nameGO = 0;					//名前(hash値)
		int m_prioGO = 0;						//プライオリティ

		bool m_isTrashTakeGO = false;	//ゴミ拾う？
		bool m_isCreatedInGOM = false; //ゲームオブジェクトマネージャーで作成された。

		UINT m_threadingFunctionFlag = tf_None;
	protected:
	};

}

