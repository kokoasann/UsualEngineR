#pragma once


namespace UER
{
	class SpriteRender :public GameObject
	{
	private:
		enum class ECameraMode : char
		{
			Usual,
			View,
			Rotate,
		};
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
			//m_isMainCameraView = true;
			m_camMode = ECameraMode::View;
		}
		/// <summary>
		/// 3dカメラの回転だけに適応。
		/// </summary>
		void CameraScaleLock()
		{
			mSprite.ViewScaleLock();
		}
		/// <summary>
		/// 
		/// </summary>
		void Mode_BillBord()
		{
			m_isBillBord = true;
		}

		void SetMulColor(const Vector4& v)
		{
			m_mulColor = v;
		}
	private:
		Sprite mSprite;											//スプライト
		Vector3 mPosition = g_vec3Zero;					//ポジション
		Quaternion mRotation = g_quatIdentity;		//ローテーション
		Vector3 mScale = g_vec3One;						//スケール
		Vector2 mPivot = {0.5f,0.5f};							//ピボット
		Vector4 m_mulColor = { 1,1,1,1 };

		bool m_isInited = false;
		bool m_isUpdated = false;

		bool m_isMainCameraView = false;
		ECameraMode m_camMode = ECameraMode::Usual;
		bool m_isBillBord = false;
	};
}