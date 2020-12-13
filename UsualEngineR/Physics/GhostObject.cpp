#include "PreCompile.h"
#include "GhostObject.h"


namespace UER
{
	void GhostObject::Release()
	{
		if (m_isRegistPhysicsWorld == true) {
			Physics().RemoveCollisionObject(m_ghost);
			m_isRegistPhysicsWorld = false;
		}
	}
	void GhostObject::Create(ICollider* collider, const Vector3& pos, const Quaternion& rot)
	{
		m_collider = collider;
		m_ghost.setCollisionShape(m_collider->GetBody());
		btTransform btTrans;
		btTrans.setOrigin({ pos.x, pos.y, pos.z });
		btTrans.setRotation({ rot.x, rot.y, rot.z, rot.w });
		m_ghost.setWorldTransform(btTrans);

		//•¨—ƒGƒ“ƒWƒ“‚É“o˜^B
		Physics().AddCollisionObject(m_ghost);
		m_isRegistPhysicsWorld = true;
	}
}
