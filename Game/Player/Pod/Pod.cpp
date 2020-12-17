#include "stdafx.h"
#include "Pod.h"
#include "../Player.h"
#include "Enemy/EnemyManager.h"
#include "../../GameSceneMenu.h"
#include "../../GameManager.h"
#include "../../Camera/GameCamera.h"
#include "../../Effect/JetEffect.h"
#include "../Attack/Projectile.h"

Pod::Pod()
{

}

Pod::~Pod()
{

}



void Pod::Release()
{
	DeleteGO(m_model);
	for (int i = 0; i < m_jetEffects.size(); i++) {
		DeleteGO(m_jetEffects[i]);
	}
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
	mid.m_tkmFilePath = "Assets/modelData/AssistantMachine/am.tkm";
	mid.m_tksFilePath = "Assets/modelData/AssistantMachine/am.tks";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(m_scale);


	//Effect
	auto jetEffect = NewGO<JetEffect>(0);
	auto jetEffect1 = NewGO<JetEffect>(0);
	JetEffect::JetEffectInitParam jeip;
	jeip.effectScale = 0.01f;
	jeip.effectScale_inv = 1.f / jeip.effectScale;
	jeip.particleScale = 5.f;
	jeip.particleLifeTime = 1.f;
	jeip.particleYUp = 300.f;
	jetEffect->Init(jeip);
	jetEffect1->Init(jeip);
	m_jetEffects.push_back(jetEffect);
	m_jetEffects.push_back(jetEffect1);


	//Bone
	m_podBones.resize(TO_INT(EnPodBone::enNumBoneType));

	{
		auto bone = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Burrel_L.001"));
		m_podBones.at(TO_INT(EnPodBone::Burrel_L1)) = bone;
	}
	{
		auto bone = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Burrel_L.002"));
		m_podBones.at(TO_INT(EnPodBone::Burrel_L2)) = bone;
	}
	{
		auto bone = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Burrel_R.001"));
		m_podBones.at(TO_INT(EnPodBone::Burrel_R1)) = bone;
	}
	{
		auto bone = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Burrel_R.002"));
		m_podBones.at(TO_INT(EnPodBone::Burrel_R2)) = bone;
	}
	{
		auto bone = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Thruster_Back"));
		m_podBones.at(TO_INT(EnPodBone::Thruster_Back)) = bone;
	}
	{
		auto bone = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Thruster_Under"));
		m_podBones.at(TO_INT(EnPodBone::Thruster_Under)) = bone;
	}

	//bullets
	auto pbm = NewGO<PlayerBulletManager>(0);
	pbm->Allocate(70);

	return true;
}


void Pod::PreUpdate()
{

}

void Pod::Update()
{
}

void Pod::PostUpdate()
{

	m_jetEffects[BACK]->SetPosition(m_podBones.at(TO_INT(EnPodBone::Thruster_Back))->GetWorldMatrix().GetTransrate());
	m_jetEffects[BACK]->SetRotation(m_podBones.at(TO_INT(EnPodBone::Thruster_Back))->GetWorldMatrix().GetRotate());

	m_jetEffects[UNDER]->SetPosition(m_podBones.at(TO_INT(EnPodBone::Thruster_Under))->GetWorldMatrix().GetTransrate());
	m_jetEffects[UNDER]->SetRotation(m_podBones.at(TO_INT(EnPodBone::Thruster_Under))->GetWorldMatrix().GetRotate());

	//m_rotation = mp_player->GetRotation();

	m_model->SetPosition(m_pos);
	m_model->SetRotation(m_rotation);


	if (GameManager::GetInstance().m_menu->IsGamePaused()) return;

	auto delta = gameTime()->GetDeltaTime();

	//printf("Pod's stamina : %f\n", m_ability.currentStamina);

	if (m_state == PodState::enIdle) {

		if (GameManager::GetInstance().m_camera->IsTargettingEnemy()) {
			//m_rotation = EnemyManager::GetEnemyManager().GetTargettingEnemy()
			auto cf = g_camera3D->GetForward();
			Quaternion rot = Quaternion::Identity;
			auto theta = atan2(cf.x, cf.z);
			theta = theta * (180.f / Math::PI);
			rot.SetRotationDegY(theta);
			m_rotation = rot;

			auto target = EnemyManager::GetEnemyManager().GetTargettingEnemy();
			if (target != nullptr) {

				Vector3 forward, right, up;
				auto& world = m_model->GetModel().GetWorldMatrix();
				forward.x = world.mat._31;
				forward.y = world.mat._32;
				forward.z = world.mat._33;
				forward.Normalize();

				right.x = world.mat._11;
				right.y = world.mat._12;
				right.z = world.mat._13;
				right.Normalize();

				up.x = world.mat._21;
				up.y = world.mat._22;
				up.z = world.mat._23;
				up.Normalize();
				
				Quaternion rotY;
				auto vecPodToTarget = target->GetPosition() - m_pos;
				vecPodToTarget.Normalize();
				auto degY = atan2(vecPodToTarget.x, vecPodToTarget.z);
				degY = Math::RadToDeg(degY);
				rotY.SetRotationDeg(up,degY);
				m_rotation = rotY;

			}
		}
		else {
			m_rotation = mp_player->GetRotation();
		}

		//auto p = mp_player->GetPosition() + m_distanceFromPlayer;
		auto addPos = m_distanceFromPlayer;
		mp_player->GetRotation().Apply(addPos);
		auto p = mp_player->GetPosition() + addPos;
		m_pos = p;

		if (mp_player->GetCurrentState() == mp_player->GetState(Player::EnState::enDead)) return;

		if (m_ability.currentStamina == 0.f) {
			m_overheat = true;
			m_overheatTimer = 0.f;
		}

		if (m_overheat) {
			m_overheatTimer += delta;
			if (m_overheatTimer >= m_OverheatTime) {
				m_overheat = false;
			}
		}

		auto& preset = mp_player->GetCurrentAttackPreset();
		bool isShooting = false;

		//long range
		if (preset == Player::EnAttackPreset::enDefault) {
			m_longRangeAttack.UpdateEffectPos();
			if (g_pad[0]->IsPress(EnButton::enButtonRB1) and !m_overheat) {
				m_longRangeAttack.Execute(mp_player);
				UseStamina(skillCosts.ProjectileCost * delta);
				isShooting = true;
			}
		}

		//Laser
		if (preset == Player::EnAttackPreset::enRemoteAttackPreset) {
			if (g_pad[0]->IsTrigger(EnButton::enButtonRB1) and !m_overheat) {
				if (m_ability.currentStamina >= skillCosts.LaserCost) {
					ShotLaserBeam();
					UseStamina(skillCosts.LaserCost);
				}
			}
		}

		//Rampage
		if (preset == Player::EnAttackPreset::enMeleePreset) {
			if (g_pad[0]->IsTrigger(EnButton::enButtonRB1) and !m_overheat) {
				if (m_ability.currentStamina >= skillCosts.RampageCost) {
					m_timer = 0.f;
					m_state = PodState::enRampage;
					UseStamina(skillCosts.RampageCost);
				}
			}
		}

		//Kamikaze
		if (preset == Player::EnAttackPreset::enExposivePreset) {
			if (g_pad[0]->IsTrigger(EnButton::enButtonRB1) and !m_overheat) {
				if (m_ability.currentStamina >= skillCosts.KamikazeCost) {
					m_timer = 0.f;
					m_state = PodState::enKamikaze;
					UseStamina(skillCosts.KamikazeCost);
				}
			}
		}


		if (!isShooting) {
			ChargeStamina(m_ability.StaminaChargeSpeed * delta);
		}

	}
	else {
		if (m_velocity.x != 0.f or m_velocity.z != 0.f) {
			Quaternion rot = Quaternion::Identity;
			auto theta = atan2(m_velocity.x, m_velocity.z);
			theta = theta * (180.f / Math::PI);
			rot.SetRotationDegY(theta);
			m_rotation = rot;
		}
	}

	if (m_state == PodState::enThrown) {
		ThrownBehave();
	}

	if (m_state == PodState::enRampage) {
		Rampage();
	}

	if (m_state == PodState::enKamikaze) {
		Kamikaze();
	}

	if (m_state == PodState::enBack) {
		BackToIdlePos();
	}

	const auto red =  1.f - (m_ability.currentStamina / m_ability.STAMINA_MAX);
	m_mulCol = Color(red * 3.f, 1.f - red,  1.f - red, 1.f);

	m_model->SetMulColor(m_mulCol);

}


void Pod::Render()
{

}

void Pod::PostRender()
{

}


void Pod::ShotLaserBeam() {
	DebugPrint_WATA("Pod : laser beam\n");

	//laser
	const float laserRange = 30.f;
	const auto& podPos = mp_player->GetPod()->GetPosition();

	auto vecLaserDir = mp_player->GetForward();
	const auto targettingEnemy = EnemyManager::GetEnemyManager().GetTargettingEnemy();
	if (targettingEnemy != nullptr)
		vecLaserDir = targettingEnemy->GetPosition() - mp_player->GetPosition();

	//auto target = GameManager::GetInstance().m_camera->GetTarget();
	//if (target != nullptr) {
//		vecLaserDir = target->GetPosition() - mp_player->GetPosition();
//	}

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

		if (std::abs(d) < laserRange) {
			EnemyManager::GetEnemyManager().GetEnemies().at(i)->ApplyDamage(m_LaserDamageAmount);
		}
	}
	//
}

void Pod::ThrownBehave() {
	const auto delta = gameTime()->GetDeltaTime();
	m_pos = m_pos + m_velocity * delta;
	m_timer += delta;

	for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
		auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
		if ((m_pos - epos).Length() < m_thrownAttackRange) {
			EnemyManager::GetEnemyManager().GetEnemies().at(i)->ApplyDamage(m_thrownAttackDamageAmount);
			m_state = PodState::enBack;
		}
	}

	if (m_timer > m_thrownTime) {
		m_state = PodState::enBack;
	}
}

void Pod::Rampage() {
	const auto delta = gameTime()->GetDeltaTime();
	m_timer += delta;

	if (EnemyManager::GetEnemyManager().GetNearestEnemy(m_pos) == nullptr) {
		m_state = PodState::enBack;
		return;
	}

	auto& epos = EnemyManager::GetEnemyManager().GetNearestEnemy(m_pos)->GetPosition();
	const float speed = 100.f;
	auto vecToEnemy = epos - m_pos;
	vecToEnemy.Normalize();
	m_pos += vecToEnemy * speed * delta;

	if ((m_pos - epos).Length() < m_thrownAttackRange) {
		EnemyManager::GetEnemyManager().GetNearestEnemy(m_pos)->ApplyDamage(m_rampagingDamageAmount);
	}

	if (m_timer > m_rampageTime) {
		m_state = PodState::enBack;
	}
}

void Pod::Kamikaze() {
	const auto delta = gameTime()->GetDeltaTime();
	m_timer += delta;

	if (EnemyManager::GetEnemyManager().GetNearestEnemy(m_pos) == nullptr) {
		m_state = PodState::enBack;
		return;
	}

	auto& epos = EnemyManager::GetEnemyManager().GetNearestEnemy(m_pos)->GetPosition();
	const float speed = 100.f;
	auto vecToEnemy = epos - m_pos;
	vecToEnemy.Normalize();
	m_pos += vecToEnemy * speed * delta;

	if ((m_pos - epos).Length() < m_thrownAttackRange) {
		//Explode
		EnemyManager::GetEnemyManager().GetNearestEnemy(m_pos)->ApplyDamage(m_kamikazeDamageAmount);
		m_state = PodState::enBack;
	}

	if (m_timer > m_rampageTime) {
		m_state = PodState::enBack;
	}
}

void Pod::BackToIdlePos() {

	const auto delta = gameTime()->GetDeltaTime();
	m_timer += delta;


	auto addPos = m_distanceFromPlayer;
	mp_player->GetRotation().Apply(addPos);
	auto idlePos = mp_player->GetPosition() + addPos;
	//auto vecToIdlePos = (mp_player->GetPosition() + mp_player->GetForward() + m_distanceFromPlayer;
	auto vecToIdlePos = idlePos - m_pos;

	auto IdlePosDir = vecToIdlePos;
	IdlePosDir.Normalize();
	auto vel = IdlePosDir * m_backSpeed * delta;

	if (vecToIdlePos.Length() < vel.Length()) {
		m_state = PodState::enIdle;
		m_pos += vecToIdlePos;
		return;
	}
	else {
		m_pos += vel;
	}
}