
#pragma once
#include "physics/SphereCollider.h"

//class CSphereCollider;

class CCameraCollisionSlover
{
public:
	CCameraCollisionSlover();
	~CCameraCollisionSlover();
	void Init(float radius);

	bool Execute(Vector3& result, const Vector3& position, const Vector3& target);
private:
	SphereCollider		m_collider;
	float				m_radius = 0.0f;
};
