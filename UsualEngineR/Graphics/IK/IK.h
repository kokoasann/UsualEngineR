#pragma once

#include "physics/SphereCollider.h"
#include "Physics/RigidBody.h"

namespace UER
{
	class IK
	{
	public:
		enum IKMode
		{
			enMode_Normal,
			enMode_Foot,		//footIK
			enMode_NoneHit,
			enMode_NoAnimHit,
		};
		/// <summary>
		/// 使用するIKの種類。
		/// </summary>
		enum UseIK
		{
			enUSE_CCD,
		};
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="effector">エフェクタボーン</param>
		/// <param name="end">エンドボーン</param>
		/// <param name="radius">コライダーの半径</param>
		IK(Skeleton* skeleton, Bone* effector, Bone* end, float radius, bool isOnRigitBody = false , const Vector3& pos = Vector3::Zero);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~IK();

		/// <summary>
		/// IKのターゲットとなる場所を更新する
		/// </summary>
		/// <param name="worldMat">モデルのワールドマトリクス</param>
		void UpdateTarget(const Matrix& worldMat);
		void UpdateTarget_Normal(const Matrix& worldMat);
		void UpdateTarget_Foot(const Matrix& worldMat);
		void UpdateTarget_NoneHit(const Matrix& worldMat);
		void UpdateTarget_NoAnimHit(const Matrix& worldMat);
		/// <summary>
		/// IKの更新。
		/// </summary>
		/// <param name="worldMat">モデルのワールドマトリクス</param>
		void Update(const Matrix& worldMat);

		/// <summary>
		/// CCD_IKを実行。
		/// </summary>
		/// <param name="worldMat">モデルのワールドマトリクス</param>
		void EXE_CCD(const Matrix& worldMat);

		/// <summary>
		/// リジッドボディの初期化。
		/// </summary>
		void InitRigidBody(const Vector3& pos);
		/// <summary>
		/// リジッドボディの更新
		/// </summary>
		/// <param name="pos"></param>
		void UpdateRigidBody(const Vector3& pos);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="mode"></param>
		void SetIKMode(IKMode mode)
		{
			m_ikMode = mode;
			if (m_ikMode == enMode_NoAnimHit)
				m_isToggleNoAnimHit = true;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="bone"></param>
		void SetEffectorBone(Bone* bone)
		{
			m_effectorBone = bone;
		}
		/// <summary>
		/// 
		/// </summary>
		Bone* GetEffectorBone() const
		{
			return m_effectorBone;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="bone"></param>
		void SetEndBone(Bone* bone)
		{
			m_endBone = bone;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Bone* GetEndBone() const
		{
			return m_endBone;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const Vector3& GetTarget() const
		{
			return m_target;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const Vector3& GetOldTarget() const
		{
			return m_oldTarget;
		}

		
		const Vector3& GetOldNewTarget() const
		{
			return m_oldNewTarget;
		}

		/// <summary>
		/// 使用されているコリジョンの半径。
		/// </summary>
		/// <returns></returns>
		float GetCollisionRadius() const
		{
			return m_radius;
		}

		/// <summary>
		/// 摩擦率をゲット
		/// </summary>
		/// <returns></returns>
		/*float GetRubbing() const
		{
			return m_rubbing;
		}*/
		/// <summary>
		/// 摩擦率をセット
		/// </summary>
		/// <param name="f">1~0が無難</param>
		/*void SetRubbing(float f)
		{
			m_rubbing = f;
		}*/

		/// <summary>
		/// 
		/// </summary>
		/// <param name="v"></param>
		void SetOffset(const Vector3& v)
		{
			m_offset = v;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="f"></param>
		/*void SetSpeed(float f)
		{
			m_speed = f;
		}*/
		/// <summary>
		/// 
		/// </summary>
		/// <param name="b"></param>
		void SetIsActive(bool b)
		{
			m_isActive = b;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool IsActive() const
		{
			return m_isActive;
		}
		void SetNextTarget(const Vector3& tar)
		{
			m_nextTarget = tar;
			m_isSetNextTarget = true;
		}

		void SetVelocity(const Vector3& v)
		{
			m_velocity = v;
		}

		void SetGravity(const float grav)
		{
			m_gravity = grav;
		}

		/*const Vector3& GetAnimMove() const
		{
			return m_animMove;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const Vector3& GetMove() const
		{
			return m_move;
		}*/

		bool IsHit() const
		{
			return m_isHit;
		}

		btCollisionObject* GetHitCollision() const
		{
			return m_hitCollision;
		}

		void SetPos(const Vector3& p);

	private:
		Skeleton* m_skeleton = 0;						//スケルトン

		bool m_isFirst = true;							//初めてか？
		Bone* m_effectorBone=0;							//エフェクタボーン
		Bone* m_endBone=0;								//エンドボーン
		float m_radius = 0.f;							//コライダーの半径。
		UseIK m_usingIK = enUSE_CCD;					//使用するIKの種類。
		IKMode m_ikMode = enMode_Normal;				//IKのモード

		SphereCollider m_collider;						//コライダー
		RigidBody m_rigidBody;							//リジッドボディ

		Vector3 m_offset = Vector3::Zero;			
		Vector3 m_target = Vector3::Zero;			//IKのターゲット
		Vector3 m_oldTarget = Vector3::Zero;		//old IKのターゲット

		Vector3 m_velocity = Vector3::Zero;
		

		//Vector3 m_rubTarget = Vector3::Zero;		//擦った先のポジション。
		//Vector3 m_move = Vector3::Zero;				//移動ベクトル
		//Vector3 m_animMove = Vector3::Zero;
		bool m_isUseLocalTarget = 1;
		//float m_speed = 1.f;							//次のターゲットに移動するときの速度(%)。1なら1フレームで移動する。0なら動かない。

		bool m_isHit = false;							//当たった？
		//float m_rubbing = 1.0f;							//摩擦率、初期値は100%　滑ることはない。

		bool m_isActive = true;							//動く？
		Vector3 m_nextTarget = Vector3::Zero;		//外部から設定された次のターゲット
		bool m_isSetNextTarget = false;					//次のターゲットがセットされた？
		bool m_isSetOldTarget = false;					//IK更新用。
		
		bool m_isUseRigidBody = true;					//リジッドボディーを使う？

		float m_gravity = 0.0f;							// 重力。
		float m_timer = 0.0f;							//重力用のタイマー
		bool m_isNextGravity = false;					
		float m_gravitPow = 0.0f;							
		Vector3 m_oldNewTarget = Vector3::Zero;		

		btCollisionObject* m_hitCollision = nullptr;

		bool m_isToggleNoAnimHit = false;
	};

}