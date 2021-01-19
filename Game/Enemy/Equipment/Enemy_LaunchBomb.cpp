#include "stdafx.h"
#include "Enemy_LaunchBomb.h"

#include "GameManager.h"
#include "Effect/ExplosionEffect.h"


struct BombContactTest :public btCollisionWorld::ContactResultCallback
{
	bool isHit = false;
	
	// ContactResultCallback ‚ð‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
	{
		int ind = colObj1Wrap->getCollisionObject()->getUserIndex();
		if (ind & enCollisionAttr_NonHit)
			return 0.f;
		isHit = true;
		return 0.f;
	}
};

struct BlastContactTest :public btCollisionWorld::ContactResultCallback
{
	bool isHit = false;

	// ContactResultCallback ‚ð‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
	{
		int ind = colObj0Wrap->getCollisionObject()->getUserIndex();
		if (ind && enCollisionAttr_NonHit)
			return 0.f;
		isHit = true;
		return 0.f;
	}
};


Enemy_LaunchBomb::Enemy_LaunchBomb()
{

}

Enemy_LaunchBomb::~Enemy_LaunchBomb()
{
	//DebugPrint_NOMOTO("DEAT");
}



void Enemy_LaunchBomb::Release()
{

}

void Enemy_LaunchBomb::OnDestroy()
{
	Physics().RemoveRigidBody(m_rigidBody);
	m_coll.Release();
}


void Enemy_LaunchBomb::Awake()
{
	m_model = NewGO<ModelRender>(0);
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";
	mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_upAxis = enUpAxisY;
	mid.m_tksFilePath = "Assets/modelData/boss/mb/mb.tks";
	m_model->Init(mid);
	
	m_effect = NewGO<ExplosionEffect>(0);
	m_effect->Init();
}

bool Enemy_LaunchBomb::Start()
{
	return true;
}

void Enemy_LaunchBomb::Init(const Vector3& pos, const Vector3& fireVelo, float size)
{
	m_coll.Create(size);
	RigidBodyInfo info;
	info.collider = &m_coll;
	info.mass = 1.f;
	info.pos = pos;
	m_rigidBody.Create(info);
	auto body = m_rigidBody.GetBody();
	body->setUserIndex(enCollisionAttr_NonHit);
	body->setLinearVelocity(btVector3(pos.x, pos.y, pos.z));
	body->setGravity({ 0,-5000,0 });
	body->applyGravity();
	Physics().AddRigidBody(m_rigidBody);

	m_model->SetScale(Vector3::One * size);
	m_model->SetPosition(pos);
}


void Enemy_LaunchBomb::PreUpdate()
{

}

void Enemy_LaunchBomb::Update()
{
	auto body = m_rigidBody.GetBody();
	body->setGravity({ 0,-5000,0 });
	body->getTotalForce();
	//body->applyGravity();
	btTransform tra;
	body->getMotionState()->getWorldTransform(tra);
	m_model->SetPosition(tra.getOrigin());
	BombContactTest res;
	Physics().ContactTest(m_rigidBody.GetBody(), res);
	if (res.isHit)
	{
		m_effect->Play();
		m_effect->SetPos(tra.getOrigin());
		auto pthis = this;
		DeleteGO(pthis);
	}
}

void Enemy_LaunchBomb::PostUpdate()
{

}



