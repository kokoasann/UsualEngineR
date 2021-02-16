#include "stdafx.h"
#include "Enemy_Bullet.h"

#include "GameManager.h"
#include "Player/Player.h"

#include "Effect/Ballistic.h"

EnemyBulletManager* g_enemyBulletManager = 0;

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
	/*ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test_criss.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);*/

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
	//DeleteGO(m_model);
}

void Enemy_Bullet::OnDestroy()
{
	//Release();
	m_sphere.Release();
	//m_model->SetActive(false);
	g_enemyBulletManager->DeadInstance(m_instanceNum);
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

	auto pair = g_enemyBulletManager->GetInstanceMatrix();
	m_instanceNum = pair.first;
	m_worldMatrix = pair.second;

	m_pos = pos;
	m_sca = Vector3::One * scale;
	m_rot = rot;

	m_worldMatrix->MakeTransform(m_pos, m_sca, m_rot);

	/*m_model->SetActive(true);
	m_model->SetPosition(pos);
	m_model->SetRotation(rot);
	m_model->SetScale(Vector3::One * m_scale);*/

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

	//Vector3 oldpos = m_model->GetPosition();
	Vector3 oldpos = m_pos;
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

			gm.m_player->ApplyDamage(m_damage, m_isStunFlag, m_vel);
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

	//m_model->SetPosition(newpos);
	m_pos = newpos;

	m_worldMatrix->SetTranspose(m_pos);
}

void Enemy_Bullet::PostUpdate()
{

}


/*/////////////////////////////////////////////////////////////////////

こっからEnemyBulletManager

/////////////////////////////////////////////////////////////////////*/

EnemyBulletManager* EnemyBulletManager::GetInstance()
{
	{
		if (g_enemyBulletManager == nullptr)
		{
			return NewGO<EnemyBulletManager>(0);
		}
		return g_enemyBulletManager;
	}
}

void EnemyBulletManager::Release()
{
}

void EnemyBulletManager::OnDestroy()
{
	Release();
}

void EnemyBulletManager::Awake()
{
	m_structuredBuff.Init(sizeof(Matrix), 1024,0);
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test_criss.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/NoAnimInstancingModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_expandShaderResoruceView = &m_structuredBuff;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);

	m_model->SetInstancingFrag(true);
	g_enemyBulletManager = this;

	m_instanceMatrix.resize(1024);
	m_usingMatrix.resize(1024);
	for (int i=0;i<m_usingMatrix.size();i++)
	{
		m_usingMatrix[i] = false;
	}
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

	std::vector<Matrix> v;
	
	int c = 0;
	for (bool b : m_usingMatrix)
	{
		if (b)
			c++;
	}
	v.reserve(1024);
	for (int i = 0; i < m_instanceMatrix.size(); i++)
	{
		if (m_usingMatrix[i])
		{
			v.push_back(m_instanceMatrix[i]);
		}
	}
	if (c == 0)
		v.push_back(Matrix::Identity);
	
	m_structuredBuff.Update(&v[0]);
	m_model->SetInstanceNum(c);
}

void EnemyBulletManager::Allocate(int num)
{
	//m_mutex.lock();
	//m_isAllocate = true;
	//m_oldNum = m_bulletList.size();
	//m_thread.Release();

	//
	//m_thread.Execute([&,num]()
	//	{
	//		//AllocateGO(num, 0, m_bulletList);
	//		/*m_instanceMatrix.resize(m_instanceMatrix.size()+num);
	//		int back = m_usingMatrix.size();
	//		m_usingMatrix.resize(m_usingMatrix.size() + num);
	//		
	//		for (int i = back; i < m_usingMatrix.size(); i++)
	//		{
	//			m_usingMatrix[i] = false;
	//		}*/
	//	});

	//m_mutex.unlock();
	//
}

std::pair<int, Matrix*> EnemyBulletManager::GetInstanceMatrix()
{
	
	for (int i = 0; i < m_instanceMatrix.size(); i++)
	{
		if (!m_usingMatrix[i])
		{
			m_usingMatrix[i] = true;
			return std::make_pair(i,&m_instanceMatrix[i]);
		}
	}
	
	m_usingMatrix.push_back(true);
	m_instanceMatrix.push_back(Matrix::Identity);

	return std::make_pair(m_instanceMatrix.size()-1, &m_instanceMatrix[m_instanceMatrix.size() - 1]);
}

void EnemyBulletManager::AddInstance()
{
}

void EnemyBulletManager::DeadInstance(int num)
{
	m_usingMatrix[num] = false;
}
