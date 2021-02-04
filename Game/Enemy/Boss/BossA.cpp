#include "stdafx.h"
#include "BossA.h"
#include "../../Effect/JetEffect.h"
#include "../State/MeleeBossState/EnemyMeleePunchState.h"
#include "../State/MeleeBossState//EnemyDashPunchState.h"
#include "../State/MeleeBossState/EnemyMeleeBattleState.h"
#include "../State/MeleeBossState/EnemyMeleeFlyState.h"
#include "../State/MeleeBossState/EnemyMeleeIdleState.h"
#include "../State/MeleeBossState/EnemyMeleeDeadState.h"
#include "../State/MeleeBossState/EnemyMeleeOverheatState.h"
#include "../State/MeleeBossState/EnemyTeleportationState.h"
#include "../State/MeleeBossState/EnemyMeleeThrusterAttackState.h"
#include "Enemy/State/EnemyStunState.h"

void BossA::Init() {

	//Model
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/boss/sp/sp.tkm";
	mid.m_tksFilePath = "Assets/modelData/boss/sp/sp.tks";
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_psfxFilePath = "Assets/shader/GBufferModelPS_ems.fx";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_emmissionMap = L"Assets/modelData/boss/sp/sp_emi.dds";

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(m_scale);
	//m_model->SetMulColor({ 20,1,1,1 });
	m_model->SetMulColor({ 0,0,0,0 });

	//Idle
	SetAnimation(TO_INT(IEnemy::EnAnimation::enIdle), "Assets/modelData/boss/sp/anim/sp_idle.tka", true);
	//Walk
	SetAnimation(TO_INT(IEnemy::EnAnimation::enWalk), "Assets/modelData/boss/sp/anim/sp_walk.tka", true);
	//Run
	SetAnimation(TO_INT(IEnemy::EnAnimation::enRun), "Assets/modelData/boss/sp/anim/sp_dush.tka", true);
	//AttackA
	SetAnimation(TO_INT(IEnemy::EnAnimation::enAttackA), "Assets/modelData/boss/sp/anim/sp_punch.tka", false);
	//AttackB
	SetAnimation(TO_INT(IEnemy::EnAnimation::enAttackB), "Assets/modelData/boss/sp/anim/sp_punch_strong.tka", false);
	//Angry
	SetAnimation(TO_INT(EnAnimEx::enAngry), "Assets/modelData/boss/sp/anim/sp_angry.tka", false);

	m_model->InitAnimation(m_animationMap, m_animationMap.size());

	PlayAnimation(EnAnimation::enIdle);

	SetState(m_stateMap.at(TO_INT(IEnemy::EnState::enIdleState)));

	m_isNormalEnemy = false;

	//Effect
	JetEffect::JetEffectInitParam smaller;
	smaller.effectScale = 0.01f;
	smaller.effectScale_inv = 1.f /  smaller.effectScale;
	smaller.particleScale = 5.f;
	smaller.particleLifeTime = 1.f;
	smaller.particleYUp = 300.f;

	JetEffect::JetEffectInitParam thrusterParam;
	thrusterParam.effectScale = 0.1f;
	thrusterParam.effectScale_inv = 1.f / thrusterParam.effectScale;;
	thrusterParam.particleScale = 5.f;
	thrusterParam.particleLifeTime = 1.f;
	thrusterParam.particleYUp = 300;

	JetEffect::JetEffectInitParam skirtParam;
	skirtParam.effectScale = 0.1f;
	skirtParam.effectScale_inv =1.f / skirtParam.effectScale;
	skirtParam.particleScale = 5;
	skirtParam.particleLifeTime = 1.f;
	skirtParam.particleYUp = 1000.f;

	for (int i = 0; i < TO_INT(EnJetBone::NumJetBone); i++) {
		auto jetEffect = NewGO<JetEffect>(0);
		m_jetEffects.push_back(jetEffect);
	}

	m_jetEffects[TO_INT(EnJetBone::ElbowR)]->Init(smaller);
	m_jetEffects[TO_INT(EnJetBone::ElbowR)]->SetGenerateFlag(false);

	m_jetEffects[TO_INT(EnJetBone::ElbowL)]->Init(smaller);
	m_jetEffects[TO_INT(EnJetBone::ElbowL)]->SetGenerateFlag(false);

	m_jetEffects[TO_INT(EnJetBone::ThrusterR)]->Init(thrusterParam);
	m_jetEffects[TO_INT(EnJetBone::ThrusterR)]->SetGenerateFlag(false);

	m_jetEffects[TO_INT(EnJetBone::ThrusterL)]->Init(thrusterParam);
	m_jetEffects[TO_INT(EnJetBone::ThrusterL)]->SetGenerateFlag(false);

	m_jetEffects[TO_INT(EnJetBone::BackR)]->Init(smaller);
	m_jetEffects[TO_INT(EnJetBone::BackR)]->SetGenerateFlag(false);

	m_jetEffects[TO_INT(EnJetBone::BackL)]->Init(smaller);
	m_jetEffects[TO_INT(EnJetBone::BackL)]->SetGenerateFlag(false);

	m_jetEffects[TO_INT(EnJetBone::Skirt)]->Init(skirtParam);
	m_jetEffects[TO_INT(EnJetBone::Skirt)]->SetGenerateFlag(true);

	m_bones.resize(TO_INT(EnJetBone::NumJetBone));

	//ElbowR
	{
		auto bone = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Bone.002_R.026"));
		m_bones.at(TO_INT(EnJetBone::ElbowR)) = bone;
	}
	//ElbowL
	{
		auto bone = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Bone.002_L.026"));
		m_bones.at(TO_INT(EnJetBone::ElbowL)) = bone;
	}
	//ThrusterR
	{
		auto bone = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Bombe_R"));
		m_bones.at(TO_INT(EnJetBone::ThrusterR)) = bone;
	}
	//ThrusterL
	{
		auto bone = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Bombe_L"));
		m_bones.at(TO_INT(EnJetBone::ThrusterL)) = bone;
	}
	//BackR
	{
		auto bone = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"BackJet_R"));
		m_bones.at(TO_INT(EnJetBone::BackR)) = bone;
	}

	//BackL
	{
		auto bone = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"BackJet_L"));
		m_bones.at(TO_INT(EnJetBone::BackL)) = bone;
	}

	//Skirt
	{
		auto bone = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"SkirtJet"));
		m_bones.at(TO_INT(EnJetBone::Skirt)) = bone;
	}

	////HandR
	//{
	//	auto bone = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"SkirtJet"));
	//	m_bones.at(TO_INT(EnJetBone::HandR)) = bone;
	//}


	//Physics
	InitCharacon(m_radius,m_height, m_position, true);

	//m_rightHandCollider.Create(m_HandRadius);


	//SE
	m_jetSE = NewGO< CSoundSource>(0);
	m_jetSE->Init(L"Assets/sound/chara/jetSe.wav", true);

}

void BossA::InitState() {
	//Init State
	m_stateMap.insert(std::make_pair(TO_INT(IEnemy::EnState::enIdleState), new EnemyMeleeIdleState()));
	m_stateMap.insert(std::make_pair(TO_INT(IEnemy::EnState::enBattleState), new EnemyMeleeBattleState()));
	m_stateMap.insert(std::make_pair(TO_INT(IEnemy::EnState::enDeadState), new EnemyMeleeDeadState()));
	m_stateMap.insert(std::make_pair(TO_INT(IEnemy::EnState::enStunState), new EnemyStunState()));
	m_stateMap.insert(std::make_pair(TO_INT(EnState::enFlyState), new EnemyMeleeFlyState()));
	m_stateMap.insert(std::make_pair(TO_INT(EnState::enTeleportation), new EnemyTeleportationState()));
	m_stateMap.insert(std::make_pair(TO_INT(EnState::enPunch), new EnemyMeleePunchState()));
	m_stateMap.insert(std::make_pair(TO_INT(EnState::enDashPunch), new EnemyDashPunchState()));
	m_stateMap.insert(std::make_pair(TO_INT(EnState::enOverheat), new EnemyMeleeOverheatState()));
	m_stateMap.insert(std::make_pair(TO_INT(EnState::enThrusterAttack), new EnemyMeleeThrusterAttackState()));
}

void BossA::Terminate() {
	DeleteGO(m_model);
	for (int i = 0; i < m_jetEffects.size(); i++) {
		DeleteGO(m_jetEffects[i]);
	}
	DeleteGO(m_jetSE);
}

void BossA::Execute() {
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);

	//Effect
	m_smokeEffect->SetPos(m_position);

	//Elbow
	m_jetEffects[TO_INT(EnJetBone::ElbowR)]->SetPosition(m_bones.at(TO_INT(EnJetBone::ElbowR))->GetWorldMatrix().GetTransrate());
	m_jetEffects[TO_INT(EnJetBone::ElbowR)]->SetRotation(m_bones.at(TO_INT(EnJetBone::ElbowR))->GetWorldMatrix().GetRotate());
	m_jetEffects[TO_INT(EnJetBone::ElbowL)]->SetPosition(m_bones.at(TO_INT(EnJetBone::ElbowL))->GetWorldMatrix().GetTransrate());
	m_jetEffects[TO_INT(EnJetBone::ElbowL)]->SetRotation(m_bones.at(TO_INT(EnJetBone::ElbowL))->GetWorldMatrix().GetRotate());
	//Thruster
	m_jetEffects[TO_INT(EnJetBone::ThrusterR)]->SetPosition(m_bones.at(TO_INT(EnJetBone::ThrusterR))->GetWorldMatrix().GetTransrate());
	m_jetEffects[TO_INT(EnJetBone::ThrusterR)]->SetRotation(m_bones.at(TO_INT(EnJetBone::ThrusterR))->GetWorldMatrix().GetRotate());
	m_jetEffects[TO_INT(EnJetBone::ThrusterL)]->SetPosition(m_bones.at(TO_INT(EnJetBone::ThrusterL))->GetWorldMatrix().GetTransrate());
	m_jetEffects[TO_INT(EnJetBone::ThrusterL)]->SetRotation(m_bones.at(TO_INT(EnJetBone::ThrusterL))->GetWorldMatrix().GetRotate());
	//Back
	m_jetEffects[TO_INT(EnJetBone::BackR)]->SetPosition(m_bones.at(TO_INT(EnJetBone::BackR))->GetWorldMatrix().GetTransrate());
	m_jetEffects[TO_INT(EnJetBone::BackR)]->SetRotation(m_bones.at(TO_INT(EnJetBone::BackR))->GetWorldMatrix().GetRotate());
	m_jetEffects[TO_INT(EnJetBone::BackL)]->SetPosition(m_bones.at(TO_INT(EnJetBone::BackL))->GetWorldMatrix().GetTransrate());
	m_jetEffects[TO_INT(EnJetBone::BackL)]->SetRotation(m_bones.at(TO_INT(EnJetBone::BackL))->GetWorldMatrix().GetRotate());
	//Skirt
	m_jetEffects[TO_INT(EnJetBone::Skirt)]->SetPosition(m_bones.at(TO_INT(EnJetBone::Skirt))->GetWorldMatrix().GetTransrate());
	m_jetEffects[TO_INT(EnJetBone::Skirt)]->SetRotation(m_bones.at(TO_INT(EnJetBone::Skirt))->GetWorldMatrix().GetRotate());


	////Physics
	////btVector3 velocity = m_forward;
	//btVector3 velocity(0, 1, 0);// = m_forward;
	//btScalar allowedCcdPenetration = 0.f;

	//btTransform t_from = m_handpos;
	//btTransform t_to = t_from;
	//t_to.setOrigin(t_to.getOrigin() + velocity);
	//btCollisionWorld::ClosestConvexResultCallback cb(t_from.getOrigin(), t_to.getOrigin());

	//btCollisionWorld::ConvexResultCallback callback();
	//Physics().ConvexSweepTest((const btConvexShape*)m_rightHandCollider.GetBody(), btStart, btEnd, callback);

	m_jetSE->SetPosition(m_position);

	//ëÃóÕÇ™Ç»Ç≠Ç»Ç¡ÇΩÇÁéÄñSÉXÉeÅ[ÉgÇ÷ëJà⁄
	if (m_ability.hp <= 0) {
		SetState(m_stateMap.at(TO_INT(IEnemy::EnState::enDeadState)));
	}
}