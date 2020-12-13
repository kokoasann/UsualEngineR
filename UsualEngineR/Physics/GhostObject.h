#pragma once

#include "Physics/ICollider.h"


namespace UER
{
	class ICollider;
	class GhostObject:public ICollider
	{
	public:
		void Release();

		void Create(ICollider* collider, const Vector3& pos, const Quaternion& rot);

		/*!
		* @brief	À•W‚ðÝ’èB
		*/
		void SetPosition(const Vector3& pos)
		{
			auto& btTrans = m_ghost.getWorldTransform();
			btVector3 btPos;
			//pos.CopyTo(btPos);
			btPos.setValue(pos.x, pos.y, pos.z);
			btTrans.setOrigin(btPos);
		}
		/*!
		* @brief	‰ñ“]‚ðÝ’èB
		*/
		void SetRotation(const Quaternion& rot)
		{
			auto& btTrans = m_ghost.getWorldTransform();
			btQuaternion btRot;
			//rot.CopyTo(btRot);
			btRot.setValue(rot.x, rot.y, rot.z);
			btRot.setW(rot.w);
			btTrans.setRotation(btRot);
		}

		btCollisionShape* GetBody()const override
		{
			return m_collider->GetBody();
		}
	private:
		ICollider* m_collider = nullptr;
		btGhostObject m_ghost;
		bool m_isRegistPhysicsWorld = false;
	};
}