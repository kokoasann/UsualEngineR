#pragma once


namespace UER
{
	class SpriteRender :public GameObject
	{
	public:
		SpriteRender() {};
		~SpriteRender() {};
		virtual void Release() override;
		void OnDestroy();

		void Init(const SpriteInitData& sid);

		void Update();

		void PostRender() override;

		/*
			mPositionにセットする
		*/
		void SetPos(const Vector3& pos)
		{
			mPosition = pos;
		}
		
		/*
			mRotationにセットする
		*/
		void SetRot(const Quaternion& rot)
		{
			mRotation = rot;
		}

		/*
			mScaleにセットする
		*/
		void SetSca(const Vector3& sca)
		{
			mScale = sca;
		}
		/*
			pivotにセットする
		*/
		void SetPivot(const Vector2& pi)
		{
			mPivot = pi;
		}
		/*
			mPositionをゲットする
		*/
		const Vector3& GetPos() const
		{
			return mPosition;
		}

		/*
			mRotationをゲットする
		*/
		const Quaternion& GetRot() const
		{
			return mRotation;
		}

		/*
			mScaleをゲットする
		*/
		const Vector3& GetSca() const
		{
			return mScale;
		}

		/// <summary>
		/// メインカメラの行列を使用する
		/// </summary>
		void MainCameraView()
		{
			m_isMainCameraView = true;
		}
		/// <summary>
		/// 
		/// </summary>
		void Mode_BillBord()
		{
			m_isBillBord = true;
		}
	private:
		Sprite mSprite;											//スプライト
		Vector3 mPosition = g_vec3Zero;					//ポジション
		Quaternion mRotation = g_quatIdentity;		//ローテーション
		Vector3 mScale = g_vec3One;						//スケール
		Vector2 mPivot = {0.5f,0.5f};							//ピボット

		bool m_isInited = false;
		bool m_isUpdated = false;

		bool m_isMainCameraView = false;
		bool m_isBillBord = false;
	};
}