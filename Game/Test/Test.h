#pragma once
#include "Physics/SphereCollider.h"
#include "Physics/BoxCollider.h"

class ThreadTest;
class Test:public GameObject
{
public:
	virtual void Release() override;
	virtual void OnDestroy() override;
	void Awake() override;
	void Update() override;
private:
	ThreadTest* m_threadTest = nullptr;
	ThreadObject m_threadObj;

	ModelRender* m_pModel;
	MeshCollider m_meshColl;
	SphereCollider m_sphere;
	BoxCollider m_box;
	RigidBody m_rb;
	

	ModelRender* m_physicsModel;
	PhysicsStaticObject m_pso;
};