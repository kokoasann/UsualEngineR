#include "stdafx.h"
#include "Projectile.h"
#include "../../Enemy/EnemyManager.h"
//#include "../../Enemy/Equipment/Enemy_Bullet.h"

PlayerBulletManager* g_playerBulletManager;

struct ProjectileSweepResult : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;
	Vector3 hitPos = Vector3::Zero;
	const Vector3& startPos;
	float dist = FLT_MAX;
	int collAttr = enCollisionAttr_None;

	ProjectileSweepResult(const Vector3& v) :
		startPos(v)
	{
	}

	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		int index = convexResult.m_hitCollisionObject->getUserIndex();
		
		if (!(index & enCollisionAttr_Character
			|| index & enCollisionAttr_Ground
			|| index & GameCollisionAttribute::BombShield)
			)
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
			if(index != -1)
				collAttr = index;
		}
		return 0.0f;
	}
};


Projectile::Projectile()
{
	/*ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test_criss.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(Vector3::One * m_scale);
	m_model->SetPosition(m_position);*/
}

Projectile::~Projectile()
{

}



void Projectile::Release()
{
	//DeleteGO(m_model);
}

void Projectile::OnDestroy()
{
	m_sphere.Release();
	//m_model->SetActive(false);
	g_playerBulletManager->DeadInstance(m_instanceNum);
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

	{
		auto pair = g_playerBulletManager->GetInstanceMatrix();
		m_instanceNum = pair.first;
		m_worldMatrix = pair.second;

		m_pos = pos;
		m_sca = Vector3::One * scale;
		m_rot = rot;

		m_worldMatrix->MakeTransform(m_pos, m_sca, m_rot);
	}

	/*m_model->SetActive(true);
	m_model->SetPosition(pos);
	m_model->SetRotation(rot);
	m_model->SetScale(Vector3::One * m_scale);*/

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

	//Vector3 oldpos = m_model->GetPosition();
	Vector3 oldpos = m_pos;
	Vector3 newpos = oldpos + m_dir * (m_speed * delta);

	//m_effect->SetPos(newpos);

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	start.setOrigin({ oldpos.x, oldpos.y, oldpos.z });
	end.setOrigin({ newpos.x, newpos.y, newpos.z });
	ProjectileSweepResult cb(oldpos);
	Physics().ConvexSweepTest(m_sphere.GetBody(), start, end, cb);

	if (cb.isHit)
	{
		if (cb.collAttr & GameCollisionAttribute::Enemy)
		{
			auto& enemyManager = EnemyManager::GetEnemyManager();
			//auto nearestEnemy = enemyManager.GetNearestEnemy(m_model->GetPosition());
			auto nearestEnemy = enemyManager.GetNearestEnemy(m_pos);
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
	m_pos = newpos;
	//m_model->SetPosition(newpos);

	m_worldMatrix->SetTranspose(m_pos);
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

void PlayerBulletManager::Awake()
{
	m_structuredBuff.Init(sizeof(Matrix), 1024, 0);
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
	g_playerBulletManager = this;

	m_instanceMatrix.resize(1024);
	m_usingMatrix.resize(1024);
	for (int i = 0; i < m_usingMatrix.size(); i++)
	{
		m_usingMatrix[i] = false;
	}
}

void PlayerBulletManager::PostUpdate()
{
	/*int activeNum = 0;
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

void PlayerBulletManager::Allocate(int num)
{
	//m_mutex.lock();
	//m_isAllocate = true;
	//m_oldNum = m_bulletList.size();
	//m_thread.Release();

	//m_thread.Execute([&, num]()
	//	{
	//		//AllocateGO(num, 0, m_bulletList);
	//	});

	//m_mutex.unlock();

}

std::pair<int, Matrix*> PlayerBulletManager::GetInstanceMatrix()
{
	for (int i = 0; i < m_instanceMatrix.size(); i++)
	{
		if (!m_usingMatrix[i])
		{
			m_usingMatrix[i] = true;
			return std::make_pair(i, &m_instanceMatrix[i]);
		}
	}

	m_usingMatrix.push_back(true);
	m_instanceMatrix.push_back(Matrix::Identity);

	return std::make_pair(m_instanceMatrix.size() - 1, &m_instanceMatrix[m_instanceMatrix.size() - 1]);
}

void PlayerBulletManager::AddInstance()
{
}

void PlayerBulletManager::DeadInstance(int num)
{
	m_usingMatrix[num] = false;
}
