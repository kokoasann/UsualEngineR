#include "stdafx.h"
#include "Bomb.h"
#include "../../Enemy/EnemyManager.h"

Bomb::Bomb()
{

}

Bomb::~Bomb()
{

}



void Bomb::Release()
{
	DeleteGO(m_model);
}

void Bomb::OnDestroy()
{
	Release();
}


void Bomb::Awake()
{

}

bool Bomb::Start()
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

	return true;
}


void Bomb::PreUpdate()
{

}

void Bomb::Update()
{
	auto delta = gameTime()->GetDeltaTime();
	m_lifeTimerSec += delta;
	m_position += m_velocity * m_speed * delta;
	m_model->SetPosition(m_position);

	bool isBombed = false;
	for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
		auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
		if ((m_position - epos).Length() < m_igniteRange) {
			//Bomb!!!
			isBombed = true;
			break;
		}
	}

	if (isBombed) {
		for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
			auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
			if ((m_position - epos).Length() < m_ExplodeDamageRange) {
				EnemyManager::GetEnemyManager().GetEnemies().at(i)->ApplyDamage(m_damage);
			}
		}
	}

	if (m_lifeTimerSec >= m_lifeSpanSec or isBombed) {
		auto obj = reinterpret_cast<GameObject*>(this);
		DeleteGO(obj);
	}
}

void Bomb::PostUpdate()
{

}


void Bomb::Render()
{

}

void Bomb::PostRender()
{

}
