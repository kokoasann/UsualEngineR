#include "stdafx.h"
#include "Pod.h"
#include "../Player.h"
#include "Enemy/EnemyManager.h"

Pod::Pod()
{

}

Pod::~Pod()
{

}



void Pod::Release()
{
	DeleteGO(m_model);
}

void Pod::OnDestroy()
{
	Release();
}


void Pod::Awake()
{

}

bool Pod::Start()
{

	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test_criss.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);

	return true;
}


void Pod::PreUpdate()
{

}

void Pod::Update()
{
	const auto delta = gameTime()->GetDeltaTime();

	if (m_state == PodState::enIdle) {
		auto p = mp_player->GetPosition() + m_distanceFromPlayer;
		m_pos = p;

		if (mp_player->GetCurrentState() == mp_player->GetState(Player::EnState::enDead)) return;

		auto& preset = mp_player->GetCurrentAttackPreset();

		if (preset == Player::EnAttackPreset::enDefault) {
			if (g_pad[0]->IsPress(EnButton::enButtonRB1)) {
				m_longRangeAttack.Execute(mp_player);
			}
		}

		if (preset == Player::EnAttackPreset::enRemoteAttackPreset) {
			if (g_pad[0]->IsTrigger(EnButton::enButtonRB1)) {
				ShotLaserBeam();
			}
		}
	}

	if (m_state == PodState::enThrown) {
		m_pos = m_pos + m_velocity * delta;
		m_thrownTimer += delta;

		for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
			auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
			if ((m_pos - epos).Length() < m_thrownAttackRange) {
				EnemyManager::GetEnemyManager().GetEnemies().at(i)->ApplyDamage(m_thrownAttackDamageAmount);
				m_state = PodState::enIdle;
			}
		}

		if (m_thrownTimer > m_thrownTime) {
			m_state = PodState::enIdle;
		}
	}

	m_model->SetPosition(m_pos);
	m_model->SetRotation(m_rotation);
}

void Pod::PostUpdate()
{

}


void Pod::Render()
{

}

void Pod::PostRender()
{

}


void Pod::ShotLaserBeam() {
	//laser
	const float laserRange = 30.f;
	const float laserDamageAmount = 200.f;
	auto vecLaserDir = mp_player->GetForward();
	const auto& podPos = mp_player->GetPod()->GetPosition();

	Vector3 N;
	N.Cross(vecLaserDir, Vector3::Up);
	N.Normalize();

	for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
		auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
		auto vecPodToEnemy = epos - podPos;

		float front = vecPodToEnemy.Dot(vecLaserDir);

		if (front < 0) {
			DebugPrint_WATA("Enemy's position is pod's back!!!!\n");
			return;
		}

		auto d = N.Dot(vecPodToEnemy);

		DebugPrint_WATA("Pod : laser beam\n");

		if (std::abs(d) < laserRange) {
			EnemyManager::GetEnemyManager().GetEnemies().at(i)->ApplyDamage(laserDamageAmount);
		}
	}
	//
}