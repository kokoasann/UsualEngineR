#include "stdafx.h"
#include "Projectile.h"
#include "../../Enemy/EnemyManager.h"

Projectile::Projectile()
{

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
	Release();
}


void Projectile::Awake()
{

}

bool Projectile::Start()
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

	return true;
}


void Projectile::PreUpdate()
{

}

void Projectile::Update()
{
	auto delta = gameTime()->GetDeltaTime();
	m_lifeTimerSec += delta;
	m_position += m_velocity * m_speed * delta;
	m_model->SetPosition(m_position);

	for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
		auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
		if ((m_position - epos).Length() < m_range) {
			EnemyManager::GetEnemyManager().GetEnemies().at(i)->ApplyDamage(m_damage);
		}
	}
	
	if (m_lifeTimerSec >= m_lifeSpanSec) {
		auto obj = reinterpret_cast<GameObject*>(this);
		DeleteGO(obj);
	}
}

void Projectile::PostUpdate()
{

}



