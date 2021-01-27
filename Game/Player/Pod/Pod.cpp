#include "stdafx.h"
#include "Pod.h"
#include "../Player.h"
#include "Enemy/EnemyManager.h"
#include "../../GameSceneMenu.h"
#include "../../GameManager.h"
#include "../../Camera/GameCamera.h"
#include "../../Effect/JetEffect.h"
#include "../Attack/Projectile.h"
#include "Effect/ExplosionEffect.h"

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

	m_explosionEffect->Stop();
	DeleteGO(m_explosionEffect);
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

	//m_smokeEffect = NewGO<SmokeEffect>(0);

	//m_smokeEffect->Init(Color::Black, Color::Black);
	//Color smokeCol = Color(0.1, 0.1, 0.1, 1.0);
	//m_smokeEffect->Init(m_smokeCol, m_smokeCol);
	//m_smokeEffect->Play();

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

	m_explosionEffect = NewGO<ExplosionEffect>(0);
	m_explosionEffect->Init();
	m_explosionEffect->SetSca(Vector3::One * 0.2f);

	m_gunSE = NewGO<CSoundSource>(0);
	m_gunSE->Init(L"Assets/sound/chara/mg.wav", true);

	return true;
}


void Pod::PreUpdate()
{

}

void Pod::Update()
{
	const auto red = 1.f - (m_ability.currentStamina / m_ability.STAMINA_MAX);
	m_mulCol = Color(red * 3.f, 1.f - red, 1.f - red, 1.f);
	m_model->SetMulColor(m_mulCol);

	//const float smokeSizeParam = 0.15f;
	//m_smokeEffect->SetSca(Vector3::One * smokeSizeParam * red);
}

void Pod::PostUpdate()
{

	m_jetEffects[BACK]->SetPosition(m_podBones.at(TO_INT(EnPodBone::Thruster_Back))->GetWorldMatrix().GetTransrate());
	m_jetEffects[BACK]->SetRotation(m_podBones.at(TO_INT(EnPodBone::Thruster_Back))->GetWorldMatrix().GetRotate());

	m_jetEffects[UNDER]->SetPosition(m_podBones.at(TO_INT(EnPodBone::Thruster_Under))->GetWorldMatrix().GetTransrate());
	m_jetEffects[UNDER]->SetRotation(m_podBones.at(TO_INT(EnPodBone::Thruster_Under))->GetWorldMatrix().GetRotate());

	//m_rotation = mp_player->GetRotation();

	if (GameManager::GetInstance().m_menu->IsGamePaused()) return;

	auto delta = gameTime()->GetDeltaTime();
	auto player = GameManager::GetInstance().GetPlayer();

	//printf("Pod's stamina : %f\n", m_ability.currentStamina);

	if (m_state == PodState::enIdle) {

		IdleRotation();
		CalcIdlePosition();

		if (player->GetCurrentState() == player->GetState(Player::EnState::enDead)) return;

		if (m_ability.currentStamina == 0.f) {
			m_overheat = true;
			m_explosionEffect->Play();
			m_overheatTimer = 0.f;
		}

		if (m_overheat) {
			m_overheatTimer += delta;
			if (m_overheatTimer >= m_OverheatTime) {
				m_overheat = false;
			}
		}

		auto& preset = player->GetCurrentAttackPreset();
		bool isShooting = false;

		//long range
		if (preset == Player::EnAttackPreset::enDefault) {
			m_longRangeAttack.UpdateEffectPos();
			if (g_pad[0]->IsPress(EnButton::enButtonRB1) and !m_overheat) {
				m_longRangeAttack.Execute(player);
				UseStamina(skillCosts.ProjectileCost * delta);
				isShooting = true;
				if (!m_gunSE->IsPlaying()) {
					m_gunSE->Play(true);
				}
			}
			else {
				if (m_gunSE->IsPlaying()) {
					m_gunSE->Stop();
				}
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
					//SE
					auto se = NewGO<CSoundSource>(0);
					se->Init(L"Assets/sound/chara/missile.wav", true);
					se->Play(false);
				}
			}
		}

		//Kamikaze
		if (preset == Player::EnAttackPreset::enExplosivePreset) {
			if (g_pad[0]->IsTrigger(EnButton::enButtonRB1) and !m_overheat) {
				if (m_ability.currentStamina >= skillCosts.KamikazeCost) {
					m_timer = 0.f;
					m_state = PodState::enKamikaze;
					UseStamina(skillCosts.KamikazeCost);
					//SE
					auto se = NewGO<CSoundSource>(0);
					se->Init(L"Assets/sound/chara/missile.wav", true);
					se->Play(false);
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

	m_model->SetPosition(m_pos);
	m_model->SetRotation(m_rotation);
	m_explosionEffect->SetPos(m_pos);
}

void Pod::Render()
{

}

void Pod::PostRender()
{

}

void Pod::ShotLaserBeam() {
	DebugPrint_WATA("Pod : laser beam\n");

	m_laserSE = NewGO<CSoundSource>(0);
	m_laserSE->Init(L"Assets/sound/chara/beam.wav", true);
	m_laserSE->Play(false);

	//laser
	const float laserRange = 30.f;
	auto player = GameManager::GetInstance().GetPlayer();
	const auto& podPos = player->GetPod()->GetPosition();

	auto vecLaserDir = player->GetForward();
	const auto targettingEnemy = EnemyManager::GetEnemyManager().GetTargettingEnemy();
	if (targettingEnemy != nullptr)
		vecLaserDir = targettingEnemy->GetPosition() - player->GetPosition();

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
		m_laserSE = NewGO<CSoundSource>(0);
		m_laserSE->Init(L"Assets/sound/chara/explosion.wav", true);
		m_laserSE->Play(false);
		m_explosionEffect->SetPos(m_pos);
		m_explosionEffect->Play();
		EnemyManager::GetEnemyManager().GetNearestEnemy(m_pos)->ApplyDamage(m_kamikazeDamageAmount);
		m_state = PodState::enBack;
	}

	if (m_timer > m_rampageTime) {
		m_state = PodState::enBack;
	}
}

void Pod::BackToIdlePos() {
	auto player = GameManager::GetInstance().GetPlayer();

	const auto delta = gameTime()->GetDeltaTime();
	m_timer += delta;


	auto addPos = m_distanceFromPlayer;
	player->GetRotation().Apply(addPos);
	auto idlePos = player->GetPosition() + addPos;
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

void Pod::IdleRotation() {

	IEnemy* target = nullptr;

	//Targetting mode's enemy
	if (GameManager::GetInstance().m_camera->IsTargettingEnemy()) {
		target = EnemyManager::GetEnemyManager().GetTargettingEnemy();
	}
	else {
		target = EnemyManager::GetEnemyManager().GetNearestEnemy(m_pos);
		if (target != nullptr) {
			const float AUTO_TARGET_RANGE = 150.f;
			if ((target->GetPosition() - m_pos).Length() >= AUTO_TARGET_RANGE) {
				target = nullptr;
			}
		}
		else {
			target = nullptr;
		}
		EnemyManager::GetEnemyManager().SetTargetEnemy(target);
	}

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

	Vector3 vecGoalForward = Vector3::One;

	if (target != nullptr) {
		vecGoalForward = target->GetPosition() - m_pos;
		vecGoalForward.Normalize();
	}
	else {
		auto player = GameManager::GetInstance().GetPlayer();
		m_rotation = player->GetRotation();
		return;
		//vecGoalForward = player->GetForward();
		//vecGoalForward.Normalize();
	}

	auto degToTar = atan2(vecGoalForward.x, vecGoalForward.z);
	auto degToForward = atan2(forward.x, forward.z);
	const float diff = degToTar - degToForward;
	const float ROT_SPEED = 350.f;
	const float delta = gameTime()->GetDeltaTime();

	auto fct = forward;
	fct.Cross(vecGoalForward);

	Quaternion rot = Quaternion::Identity;

	const float deadVal = 0.05;
	if (fct.y > deadVal) {
		rot.SetRotationDegY(ROT_SPEED * delta);
		m_rotation.Multiply(rot);
	}
	else if (fct.y < -deadVal) {
		rot.SetRotationDegY(-ROT_SPEED * delta);
		m_rotation.Multiply(rot);
	}
	else {
		Quaternion rotY;
		auto degY = atan2(vecGoalForward.x, vecGoalForward.z);
		degY = Math::RadToDeg(degY);
		rotY.SetRotationDeg(up, degY);
		m_rotation = rotY;
	}
}

void Pod::CalcIdlePosition() {

	auto player = GameManager::GetInstance().GetPlayer();
	auto addPos = m_distanceFromPlayer;

	if (GameManager::GetInstance().GetGameCamera()->IsTargettingEnemy()) {
		g_camera3D->GetCameraRotation().Apply(addPos);
	}
	else {
		player->GetRotation().Apply(addPos);
	}

	auto idlePos = player->GetPosition() + addPos;
	m_pos = idlePos;

}