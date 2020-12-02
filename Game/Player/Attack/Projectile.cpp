#include "stdafx.h"
#include "Projectile.h"
#include "../../Enemy/EnemyManager.h"
#include "../../Enemy/Equipment/Enemy_Bullet.h"

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


Projectile::Projectile()
{
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test_criss.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(Vector3::One * m_scale);
	m_model->SetPosition(m_position);
}

Projectile::~Projectile()
{

}



void Projectile::Release()
{
	DeleteGO(m_model);
}

void Projectile::OnDestroy()
{
	m_sphere.Release();
	m_model->SetActive(false);
}


void Projectile::Awake()
{

}

void Projectile::Init(const Vector3& pos, float scale, const Vector3& dir, float speed, float timeLimit, float attackRenge){
	m_dir = dir;
	m_speed = speed;
	m_scale = scale;
	m_lifeSpanSec = timeLimit;
	m_range = attackRenge;

	Quaternion rot;
	rot.SetRotation(Vector3::AxisZ * -1.f, m_dir);

	m_model->SetActive(true);
	m_model->SetPosition(pos);
	m_model->SetRotation(rot);
	m_model->SetScale(Vector3::One * m_scale);

	m_sphere.Create(scale * 0.5);

	m_lifeTimerSec = 0.f;
	rot.SetRotation(Vector3::AxisY, m_dir * -1.f);
}

bool Projectile::Start()
{
	return true;
}


void Projectile::PreUpdate()
{

}

void Projectile::Update()
{
	auto delta = gameTime()->GetDeltaTime();
	m_lifeTimerSec += delta;
	//m_position += m_velocity * m_speed * delta;
	//m_model->SetPosition(m_position);

	//for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
	//	auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
	//	if ((m_position - epos).Length() < m_range) {
	//		EnemyManager::GetEnemyManager().GetEnemies().at(i)->ApplyDamage(m_damage);
	//	}
	//}
	//
	//if (m_lifeTimerSec >= m_lifeSpanSec) {
	//	auto obj = reinterpret_cast<GameObject*>(this);
	//	DeleteGO(obj);
	//}

	//

	Vector3 oldpos = m_model->GetPosition();
	Vector3 newpos = oldpos + m_dir * (m_speed * delta);

	//m_effect->SetPos(newpos);

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	start.setOrigin({ oldpos.x, oldpos.y, oldpos.z });
	end.setOrigin({ newpos.x, newpos.y, newpos.z });
	SweepResult cb(oldpos);
	Physics().ConvexSweepTest(m_sphere.GetBody(), start, end, cb);

	if (cb.isHit)
	{
		if (cb.collAttr & GameCollisionAttribute::Enemy)
		{
			auto& enemyManager = EnemyManager::GetEnemyManager();
			auto nearestEnemy = enemyManager.GetNearestEnemy(m_model->GetPosition());
			nearestEnemy->ApplyDamage(m_damage);
		}
		if (!(cb.collAttr & GameCollisionAttribute::Player)) {
			auto pthis = this;
			DeleteGO(pthis);
			return;
		}
	}

	if (m_lifeTimerSec >= m_lifeSpanSec)
	{
		auto pthis = this;
		DeleteGO(pthis);
		return;
	}

	m_position = newpos;
	m_model->SetPosition(newpos);
}

void Projectile::PostUpdate()
{

}



////////////////////////////////
//PlayerBulletManager
////////////////////////////////

void PlayerBulletManager::Release()
{
}

void PlayerBulletManager::OnDestroy()
{
	Release();
}

void PlayerBulletManager::PostUpdate()
{
	int activeNum = 0;
	for (auto b : m_bulletList)
	{
		if (!b->IsDead())
		{
			activeNum++;
		}
	}
	if ((m_bulletList.size() - activeNum) <= m_nextAllocateActiveNum && m_thread.IsEnd())
	{
		Allocate(m_allocElementNum);
	}
}

void PlayerBulletManager::Allocate(int num)
{
	m_mutex.lock();
	m_isAllocate = true;
	m_oldNum = m_bulletList.size();
	m_thread.Release();

	m_thread.Execute([&, num]()
		{
			AllocateGO(num, 0, m_bulletList);
		});

	m_mutex.unlock();

}
