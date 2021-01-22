#include "stdafx.h"
#include "Enemy_LaunchBomb.h"
#include "Enemy/IEnemy.h"
#include "GameManager.h"
#include "Effect/ExplosionEffect.h"

struct SweepResult : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;
	Vector3 hitPos = Vector3::Zero;
	const Vector3& startPos;
	float dist = FLT_MAX;
	int collAttr = enCollisionAttr_None;

	SweepResult(const Vector3& v) :
		startPos(v)
	{

	}

	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		int index = convexResult.m_hitCollisionObject->getUserIndex();
		//キャラコンでもグラウンドでもない場合早期出社。
		if (!(index & enCollisionAttr_Character
			|| index & enCollisionAttr_Ground))
		{
			return 0.f;
		}

		isHit = true;
		Vector3 hitp = *(Vector3*)&convexResult.m_hitPointLocal;
		Vector3 div = startPos - hitp;
		float dis = div.Length();
		if (dis < dist)
		{
			hitPos = *(Vector3*)&convexResult.m_hitPointLocal;
			dist = dis;
			collAttr = index;
		}
		return 0.0f;
	}
};


struct BombContactTest :public btCollisionWorld::ContactResultCallback
{
	bool isHit = false;
	bool isHitPlayer = false;
	std::vector<IEnemy*> enemyList;
	
	// ContactResultCallback を介して継承されました
	virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
	{
		int ind = colObj1Wrap->getCollisionObject()->getUserIndex();
		if (ind & enCollisionAttr_NonHit)
			return 0.f;
		

		if (ind & GameCollisionAttribute::Enemy)
		{
			enemyList.push_back(reinterpret_cast<IEnemy*>(colObj1Wrap->getCollisionObject()->getUserPointer()));
			isHit = true;
		}
		else if (ind & GameCollisionAttribute::Player)
		{
			isHitPlayer = true;
			isHit = true;
		}

		return 0.f;
	}
};

struct BlastContactTest :public btCollisionWorld::ContactResultCallback
{
	bool isHit = false;

	// ContactResultCallback を介して継承されました
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
	//OnDestroy();
}



void Enemy_LaunchBomb::Release()
{
	DeleteGO(m_model);
	if (m_isDestroy)
		return;
	Physics().RemoveRigidBody(m_rigidBody);
	m_rigidBody.Release();
	m_coll.Release();
	m_ghost.Release();
	m_fireColl.Release();
}

void Enemy_LaunchBomb::OnDestroy()
{
	if (m_isDestroy)
		return;
	Physics().RemoveRigidBody(m_rigidBody);
	m_rigidBody.Release();
	m_coll.Release();
	m_model->SetActive(false);

	//Physics().RemoveCollisionObject((btGhostObject&)*m_ghost.GetBody());
	m_ghost.Release();
	m_fireColl.Release();

	m_isDestroy = true;
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
	m_model->SetActive(true);
}

bool Enemy_LaunchBomb::Start()
{
	return true;
}

void Enemy_LaunchBomb::Init(const Vector3& pos, const Vector3& fireVelo, float size,float fireRange,float damage, float power)
{
	m_damage = damage;
	m_fireRange = fireRange;
	m_power = power;
	m_fireColl.Create(fireRange);
	m_ghost.Create(&m_fireColl, Vector3::Zero, Quaternion::Identity);

	m_coll.Create(size);
	RigidBodyInfo info;
	info.collider = &m_coll;
	info.mass = 1.f;
	info.pos = pos;
	m_rigidBody.Create(info);
	auto body = m_rigidBody.GetBody();
	body->setUserIndex(enCollisionAttr_NonHit);
	body->setLinearVelocity(btVector3(fireVelo.x, fireVelo.y, fireVelo.z));
	body->setGravity({ 0,-500,0 });
	body->applyGravity();
	Physics().AddRigidBody(m_rigidBody);

	m_model->SetScale(Vector3::One * size);
	m_model->SetPosition(pos);

	m_isDestroy = false;
	m_isHit = false;
}


void Enemy_LaunchBomb::PreUpdate()
{

}

void Enemy_LaunchBomb::Update()
{
	if (m_isHit)
	{
		auto pl = GameManager::GetInstance().GetPlayer();
		auto v = pl->GetPosition() - m_hitPos;
		float len = v.Length();
		
		v.Normalize();
		v.y = 1.f;
		float t = 1.f - min(len / m_fireRange, 1.f);
		v *= m_fireRange * t * m_power;

		for (int i = 0; i < m_ghost.GetGhost()->getNumOverlappingObjects(); i++)
		{
			auto o = m_ghost.GetGhost()->getOverlappingObject(i);
			int ind = o->getUserIndex();
			if (ind & GameCollisionAttribute::Enemy)
			{
				auto e = (IEnemy*)o->getUserPointer();
				e->ApplyDamage(m_damage * t, true, v);
			}
			else if (ind & GameCollisionAttribute::Player)
			{
				
				pl->ApplyDamage(m_damage * t, true, v);
			}
		}
		
		auto pthis = this;
		DeleteGO(pthis);
		return;
	}
	auto body = m_rigidBody.GetBody();
	body->setGravity({ 0,-500,0 });
	body->getTotalForce();
	//body->applyGravity();
	btTransform tra,end;
	body->getMotionState()->getWorldTransform(tra);
	m_model->SetPosition(tra.getOrigin());
	end = tra;
	auto velo = body->getLinearVelocity() * gameTime()->GetDeltaTime();
	end.setOrigin(tra.getOrigin() + body->getLinearVelocity()*gameTime()->GetDeltaTime());
	SweepResult sres(tra.getOrigin());
	Physics().ConvexSweepTest(m_coll.GetBody(), tra,end,sres);
	
	if (sres.isHit)
	{
		m_ghost.SetPosition(sres.hitPos);
		m_effect->Play();
		m_effect->SetPos(sres.hitPos);
		m_hitPos = sres.hitPos;
		m_isHit = true;

		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/explosion.wav");
		se->Play(false);
		se->SetVolume(1.);
		return;
		


		//m_ghost.SetPosition(sres.hitPos);
		//BombContactTest res;
		//Physics().ContactTest(m_ghost.GetGhost(), res);
		//if (res.isHit)
		//{
		//	auto pl = GameManager::GetInstance().GetPlayer();
		//	auto v = pl->GetPosition() - sres.hitPos;
		//	float len = v.Length();
		//	v.y = 1.f;
		//	v.Normalize();
		//	float t = 1.f - min(len / m_fireRange, 1.f);
		//	v *= m_fireRange * t;
		//	if (res.isHitPlayer)
		//	{
		//		pl->ApplyDamage(m_damage, true, v);
		//	}
		//	for (auto e : res.enemyList)
		//	{
		//		e->ApplyDamage(m_damage, true, v);
		//	}
		//}
	}

	
	/*BombContactTest res;
	Physics().ContactTest(m_rigidBody.GetBody(), res);
	if (res.isHit)
	{
		m_effect->Play();
		m_effect->SetPos(tra.getOrigin());
		auto pthis = this;
		DeleteGO(pthis);
	}*/
}

void Enemy_LaunchBomb::PostUpdate()
{

}



