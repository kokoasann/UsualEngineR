/*!
* @brief	キャラクタコントローラー。
*/

#pragma once

#include "Physics/CapsuleCollider.h"
#include "Physics/RigidBody.h"

namespace UER
{
	/*!
	* @brief	キャラクタコントローラー。
	*/
	class CharacterController {
	public:
		CharacterController() {

		}
		~CharacterController()
		{
			RemoveRigidBoby();
		}
		/*!
			* @brief	初期化。
			*@param[in]	radius		カプセルコライダーの半径。
			*@param[in]	height		カプセルコライダーの高さ。
			*@param[in]	position	初期位置。
			*/
		void Init(float radius, float height, const Vector3& position, bool isUseRigidBody=true);
		/*!
			* @brief	実行。
			*@param[in]	deltaTime		経過時間。単位は秒。
			*@param[in, out]	moveSpeed		移動速度。内部で重力加速が計算され、その結果がmoveSpeedに反映されます。
			*@return 移動後のキャラクターの座標。
			*/
		const Vector3& Execute(float deltaTime, const Vector3& moveSpeed);

		void ExecuteWall(Vector3& nowPos, Vector3& nextPos, const Vector3& originalXZDir, float& Ypos);
		void ExecuteFloor(Vector3& nowPos, Vector3& nextPos, const Vector3& originalXZDir, float& Ypos);
		/*!
			* @brief	座標を取得。
			*/
		const Vector3& GetPosition() const
		{
			return m_position;
		}
		/*!
			* @brief	座標を設定。
			*/
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}

		/*!
			* @brief	ジャンプ中か判定
			*/
		bool IsJump() const
		{
			return m_isJump;
		}
		/*!
		* @brief	地面上にいるか判定。
		*/
		bool IsOnGround() const
		{
			return m_isOnGround;
		}
		/*!
		* @brief	コライダーを取得。
		*/
		CapsuleCollider* GetCollider()
		{
			return &m_collider;
		}
		/*!
		* @brief	剛体を取得。
		*/
		RigidBody* GetRigidBody()
		{
			return &m_rigidBody;
		}

		/// <summary>
		/// コライダーの属性の追加。
		/// </summary>
		/// <param name="ca"></param>
		template<class T>
		void AddCollisionAttribute(T ca)
		{
			if (!m_isUseRigidBody)
				return;
			m_rigidBody.GetBody()->setUserIndex(m_rigidBody.GetBody()->getUserIndex() | static_cast<int>(ca));
		}

		/// <summary>
		/// キャラコンにポインターを設定。
		/// </summary>
		/// <param name="p"></param>
		void SetUserPointer(void* p)
		{
			if (!m_isUseRigidBody)
				return;
			m_rigidBody.GetBody()->setUserPointer(p);
		}

		/*!
		* @brief	剛体を物理エンジンから削除。。
		*/
		void RemoveRigidBoby();

		float GetRadius() const
		{
			return m_radius;
		}
		float GetHeight() const
		{
			return m_height;
		}
	private:
		Vector3 			m_position = g_vec3Zero;	//座標。
		bool 				m_isJump = false;				//ジャンプ中？
		bool				m_isOnGround = true;			//地面の上にいる？
		CapsuleCollider		m_collider;						//コライダー。
		float				m_radius = 0.0f;
		float				m_height = 0.0f;
		RigidBody			m_rigidBody;					//剛体。
		bool				m_isUseRigidBody = true;		//リジッドボデューを使う？
		bool				m_isRemoveRigidBody = false;

		float				m_offsetXZ = 0.0f;
		float				m_offsetY = 0.0f;
	};
}