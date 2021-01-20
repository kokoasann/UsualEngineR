#include "stdafx.h"
#include "Bomb.h"
//#include "../../Enemy/EnemyManager.h"
#include "Enemy/EnemyManager.h"
#include "Effect/ExplosionEffect.h"

Bomb::Bomb()
{

}

Bomb::~Bomb()
{

}

void Bomb::Release()
{
	m_explosionEffect->Stop();
	DeleteGO(m_explosionEffect);
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

	m_se = NewGO< CSoundSource>(0);
	m_se->Init(L"Assets/sound/chara/explosion.wav", true);

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

	m_explosionEffect = NewGO<ExplosionEffect>(0);
	m_explosionEffect->Init();
	m_explosionEffect->SetSca(Vector3::One * 0.2f);

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

	if (isBombed == false and (m_position - EnemyManager::GetEnemyManager().GetNearestEnemy(m_position)->GetPosition()).Length() < m_igniteRange) {
		isBombed = true;
		m_explosionEffect->SetPos(m_position);
		m_explosionEffect->Play();
		m_isExploded = true;
		m_se->Play(false);

		for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
			auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
			if ((m_position - epos).Length() < m_ExplodeDamageRange) {
				auto vecKb = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition() - m_position;
				vecKb.y = 0;
				vecKb.Normalize();
				vecKb.y = 2.f;
				vecKb.Normalize();
				vecKb *= m_knockBackPower;
				EnemyManager::GetEnemyManager().GetEnemies().at(i)->ApplyDamage(m_damage, true, vecKb);
			}
		}
	}

	//for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
	//	auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
	//	if ((m_position - epos).Length() < m_igniteRange) {
	//		//Bomb!!!
	//		isBombed = true;
	//		break;
	//	}
	//}

	//if (isBombed) {
	//	for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
	//		auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
	//		if ((m_position - epos).Length() < m_ExplodeDamageRange) {
	//			EnemyManager::GetEnemyManager().GetEnemies().at(i)->ApplyDamage(m_damage);
	//		}
	//	}
	//}
	if (m_isExploded) {
		m_destroyTimer += delta;
		if (m_destroyTimer >= m_destroyInterval) {
			auto obj = reinterpret_cast<GameObject*>(this);
			printf("delete bomb : bomb and  destroy!");
			DeleteGO(obj);
		}
	}
	else if (m_lifeTimerSec >= m_lifeSpanSec) {
		auto obj = reinterpret_cast<GameObject*>(this);
		printf("delete bomb :   timer destroy!");
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
