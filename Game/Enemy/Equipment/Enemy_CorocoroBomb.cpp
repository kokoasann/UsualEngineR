#include "stdafx.h"
#include "Enemy_CorocoroBomb.h"
#include "Effect/ExplosionEffect.h"
#include "Enemy/IEnemy.h"
#include "GameManager.h"
#include "Effect/ExplosionEffect.h"

Enemy_CorocoroBomb::Enemy_CorocoroBomb()
{

}

Enemy_CorocoroBomb::~Enemy_CorocoroBomb()
{

}

void Enemy_CorocoroBomb::Release()
{
	DeleteGO(m_model);
	if (!m_isHit)
	{
		Physics().RemoveRigidBody(m_rigidBody);
		m_rigidBody.Release();
		m_coll.Release();
	}
	if(!m_isDestroy)
	{
		m_ghost.Release();
		m_fireColl.Release();
	}
}

void Enemy_CorocoroBomb::OnDestroy()
{
	m_ghost.Release();
	m_fireColl.Release();
	m_isDestroy = true;
}


void Enemy_CorocoroBomb::Awake()
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

bool Enemy_CorocoroBomb::Start()
{
	return true;
}

void Enemy_CorocoroBomb::Init(const Vector3& pos, const Vector3& fireVelo, float size, float fireRange, float damage, float power,float timeLimit)
{
	m_timeLimit = timeLimit;
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
	body->setGravity({ 0,-200,0 });
	body->applyGravity();
	Physics().AddRigidBody(m_rigidBody);

	m_model->SetScale(Vector3::One * size);
	m_model->SetPosition(pos);

	m_isDestroy = false;
	m_isHit = false;
	m_timer = 0.f;
}


void Enemy_CorocoroBomb::PreUpdate()
{

}

void Enemy_CorocoroBomb::Update()
{
	if (m_isHit)
	{
		auto pl = GameManager::GetInstance().GetPlayer();
		auto v = pl->GetPosition() - m_hitpos;
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

	float dtime = gameTime()->GetDeltaTime();
	m_timer += dtime;

	auto body = m_rigidBody.GetBody();
	//body->setGravity({ 0,-200,0 });
	body->getTotalForce();
	btTransform tra, end;
	body->getMotionState()->getWorldTransform(tra);
	Vector3 pos = tra.getOrigin();
	m_model->SetPosition(pos);
	m_model->SetRotation(tra.getRotation());

	
	if (m_timer >= m_timeLimit)
	{
		m_effect->Play();
		m_effect->SetPos(pos);

		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/explosion.wav");
		se->Play(false);
		se->SetVolume(1.);

		m_ghost.SetPosition(pos);
		m_hitpos = pos;
		m_isHit = true;

		Physics().RemoveRigidBody(m_rigidBody);
		m_rigidBody.Release();
		m_coll.Release();
		m_model->SetActive(false);
	}
}

void Enemy_CorocoroBomb::PostUpdate()
{

}



