#include "stdafx.h"
#include "CCameraCollisionSlover.h"
#include "physics/SphereCollider.h"

struct SConvexSweepCallback : public btCollisionWorld::ClosestConvexResultCallback
{
public:
	Vector3 m_rayDir;
	SConvexSweepCallback(Vector3 rayDir) :
		btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f)),
		m_rayDir(rayDir) {}
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		Vector3 normal;
		normal.Set(convexResult.m_hitNormalLocal);

		if (convexResult.m_hitCollisionObject->getUserIndex() == 1) {
			return 1.0f;
		}
		return btCollisionWorld::ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
	}
};

CCameraCollisionSlover::CCameraCollisionSlover()
{
}


CCameraCollisionSlover::~CCameraCollisionSlover()
{
}
void CCameraCollisionSlover::Init(float radius)
{
	m_radius = radius;
	m_collider.Create(radius);
}
bool CCameraCollisionSlover::Execute(Vector3& result, const Vector3& position, const Vector3& target)
{
	result = position;
	Vector3 vWk;
	vWk.Subtract(target, position);
	if (vWk.LengthSq() < FLT_EPSILON) {
		return false;
	}
	vWk.Normalize();
	btTransform btStart, btEnd;
	btStart.setIdentity();
	btEnd.setIdentity();
	btStart.setOrigin(btVector3(target.x, target.y, target.z));
	btEnd.setOrigin(btVector3(position.x, position.y, position.z));
	SConvexSweepCallback callback(vWk);
	Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), btStart, btEnd, callback);
	if (callback.hasHit()) {
		Vector3 vHitPos;
		vHitPos.Set(callback.m_hitPointWorld);
		Vector3 vOffset;
		vOffset.Set(callback.m_hitNormalWorld);
		vOffset.Scale(m_radius);
		result.Add(vHitPos, vOffset);
		return true;
	}
	return false;
}
