#include "stdafx.h"
#include "Enemy_Bullet.h"

#include "GameManager.h"
#include "Player/Player.h"

#include "Effect/Ballistic.h"

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


Enemy_Bullet::Enemy_Bullet()
{
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test_criss.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);

	/*m_effect = NewGO<Ballistic>(0);
	m_effect->Init();*/
	
	//m_model->SetPosition(pos);
	//m_model->SetRotation(rot);

	//m_sphere.Create(scale * 0.5);
}

Enemy_Bullet::~Enemy_Bullet()
{

}



void Enemy_Bullet::Release()
{
	DeleteGO(m_model);
}

void Enemy_Bullet::OnDestroy()
{
	//Release();
	m_sphere.Release();
	m_model->SetActive(false);
	//m_effect->Stop();
}

void Enemy_Bullet::Init(const Vector3& pos, float scale, const Vector3& dir, float speed, float timeLimit, float attackRenge)
{
	m_dir = dir;
	m_speed = speed;
	m_scale = scale;
	m_timeLimit = timeLimit;
	m_attackRange = attackRenge;

	Quaternion rot;
	rot.SetRotation(Vector3::AxisZ * -1.f, m_dir);

	m_model->SetActive(true);
	m_model->SetPosition(pos);
	m_model->SetRotation(rot);
	m_model->SetScale(Vector3::One * m_scale);

	m_sphere.Create(scale * 0.5);

	m_timer = 0.f;

	rot.SetRotation(Vector3::AxisY, m_dir*-1.f);
	/*m_effect->SetRot(rot);
	m_effect->SetPos(pos);
	m_effect->Play();*/
}


void Enemy_Bullet::Awake()
{

}

bool Enemy_Bullet::Start()
{
	return true;
}


void Enemy_Bullet::PreUpdate()
{

}

void Enemy_Bullet::Update()
{
	float dtime = gameTime()->GetDeltaTime();

	m_timer += dtime;

	Vector3 oldpos = m_model->GetPosition();
	Vector3 newpos = oldpos + m_dir * (m_speed * dtime);

	//m_effect->SetPos(newpos);

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	start.setOrigin({ oldpos.x, oldpos.y, oldpos.z });
	end.setOrigin({ newpos.x, newpos.y, newpos.z });
	SweepResult sr(oldpos);
	Physics().ConvexSweepTest(m_sphere.GetBody(), start, end, sr);
	
	if (sr.isHit)
	{
		if (sr.collAttr & GameCollisionAttribute::Player)
		{
			auto& gm = GameManager::GetInstance();

			gm.m_player->ApplyDamage(m_damage);
		}
		else if (sr.collAttr & GameCollisionAttribute::Enemy)
		{

		}
		else
		{
			
		}
		auto pthis = this;
		DeleteGO(pthis);
		return;
	}
	if (m_timer >= m_timeLimit)
	{
		auto pthis = this;
		DeleteGO(pthis);
		return;
	}

	m_model->SetPosition(newpos);

	
}

void Enemy_Bullet::PostUpdate()
{

}


/*/////////////////////////////////////////////////////////////////////

こっからEnemyBulletManager

/////////////////////////////////////////////////////////////////////*/

void EnemyBulletManager::Release()
{
}

void EnemyBulletManager::OnDestroy()
{
	Release();
}

void EnemyBulletManager::PostUpdate()
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

	/*if (m_isAllocate && m_thread.IsEnd())
	{
		for (int i = m_oldNum; i < m_bulletList.size(); i++)
		{
			DeleteGO(m_bulletList[i]);
		}
		m_isAllocate = false;
	}*/
}

void EnemyBulletManager::Allocate(int num)
{
	m_mutex.lock();
	m_isAllocate = true;
	m_oldNum = m_bulletList.size();
	m_thread.Release();

	m_thread.Execute([&,num]()
		{
			AllocateGO(num, 0, m_bulletList);
		});

	m_mutex.unlock();
	
}
