#pragma once


namespace UER
{
	class SpriteRender :public GameObject
	{
	public:
		SpriteRender() {};
		~SpriteRender() {};

		void OnDestroy();

		void Init(const SpriteInitData& sid);

		void Update();

		void PostRender() override;

		/*
			mPositionにセットする
		*/
		void SetPos(Vector3 pos)
		{
			mPosition = pos;
		}
		
		/*
			mRotationにセットする
		*/
		void SetRot(Quaternion rot)
		{
			mRotation = rot;
		}

		/*
			mScaleにセットする
		*/
		void SetSca(Vector3 sca)
		{
			mScale = sca;
		}
		/*
			pivotにセットする
		*/
		void SetPivot(Vector2 pi)
		{
			mPivot = pi;
		}
		/*
			mPositionをゲットする
		*/
		Vector3 GetPos()
		{
			return mPosition;
		}

		/*
			mRotationをゲットする
		*/
		Quaternion GetRot()
		{
			return mRotation;
		}

		/*
			mScaleをゲットする
		*/
		Vector3 GetSca()
		{
			return mScale;
		}
	private:
		Sprite mSprite;											//スプライト
		Vector3 mPosition = g_vec3Zero;					//ポジション
		Quaternion mRotation = g_quatIdentity;		//ローテーション
		Vector3 mScale = g_vec3One;						//スケール
		Vector2 mPivot = {0.5f,0.5f};							//ピボット

	};
}