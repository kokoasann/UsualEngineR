#include "stdafx.h"
#include "Boss_Fatman.h"
#include "Enemy/State/EnemyDeadState.h"
#include "Enemy/State/EnemyIdleState.h"
#include "Enemy/State/BossFatmanState/Boss_FatmanMainState.h"
#include "Enemy/State/BossFatmanState/Boss_FatmanBeamState.h"
#include "Enemy/State/BossFatmanState/Boss_FatmanChargeBeamState.h"
#include "Enemy/State/BossFatmanState/Boss_FatmanShootingState.h"
#include "Enemy/State/BossFatmanState/Boss_FatmanTakeDistanceState.h"
#include "Enemy/State/BossFatmanState/Boss_FatmanTackleState.h"
#include "Enemy/State/BossFatmanState/Boss_FatmanTakeDistanceState.h"
#include "Enemy/State/BossFatmanState/Boss_FatmanStunState.h"
#include "Enemy/State/BossFatmanState/Boss_FatmanChargeBeamAndShootingState.h"
#include "Enemy/State/BossFatmanState/Boss_FatmanDeadState.h"

EnBattlePhase Boss_Fatman::m_battlePhase = EnBattlePhase::Normal;

Boss_Fatman::Boss_Fatman()
{

}

Boss_Fatman::~Boss_Fatman()
{

}

void Boss_Fatman::Init()
{
	//Model
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/boss/lf/lf.tkm";
	mid.m_tksFilePath = "Assets/modelData/boss/lf/lf.tks";
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_upAxis = enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(Vector3::One * m_scale);

	InitAnimation();

	//State
	SetState(m_stateMap[TO_INT(IEnemy::EnState::enIdleState)]);

	//Physics
	InitCharacon(m_radius, m_height, m_position, true);
}

void Boss_Fatman::InitAnimation()
{
	SetAnimation(TO_INT(IEnemy::EnAnimation::enIdle), "Assets/modelData/boss/lf/anim/lf_idle.tka", true);
	SetAnimation(TO_INT(Boss_Fatman::EnAnimEX::enAngry), "Assets/modelData/boss/lf/anim/lf_angry.tka", true);
	SetAnimation(TO_INT(Boss_Fatman::EnAnimEX::enbackStep), "Assets/modelData/boss/lf/anim/lf_backStep.tka", true);

	m_model->InitAnimation(m_animationMap, m_animationMap.size());
}

void Boss_Fatman::InitState()
{
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enDeadState), new Boss_FatmanDeadState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enStunState), new Boss_FatmanStunState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enIdleState), new EnemyIdleState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enBattleState), new Boss_FatmanMainState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enAttackA), new Boss_FatmanBeamState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enAttackB), new Boss_FatmanChargeBeamState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(EnStateEX::enAttackC), new Boss_FatmanShootingState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(EnStateEX::enAttackD), new Boss_FatmanTackleState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(EnStateEX::enTakeDistance), new Boss_FatmanTakeDistanceState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(EnStateEX::enAttackE), new Boss_FatmanChargeBeamAndShootingState());
		m_stateMap.insert(p);
	}
}

void Boss_Fatman::InitIK()
{
	const float radius = 0.5f;
	auto ske = m_model->GetModel().GetSkelton();
	{
		IK* ik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Beam_IK_R")), 1, radius);
		ik->SetIKMode(IK::enMode_NoneHit);

		SetIK(TO_INT(EnIK::enArm_R), ik);
	}
	{
		IK* ik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Beam_IK_L")), 1, radius);
		ik->SetIKMode(IK::enMode_NoneHit);
		SetIK(TO_INT(EnIK::enArm_L), ik);
	}
	{
		IK* ik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Barrel_IK_L")), 1, radius);
		ik->SetIKMode(IK::enMode_NoneHit);

		SetIK(TO_INT(EnIK::enFoot_L), ik);
	}
	{
		IK* ik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Barrel_IK_R")), 1, radius);
		ik->SetIKMode(IK::enMode_NoneHit);

		SetIK(TO_INT(EnIK::enFoot_R), ik);
	}
}

void Boss_Fatman::Execute()
{
	m_model->SetPosition(m_position);
	//m_model->SetRotation(m_rotation);

	//体力がなくなったら死亡ステートへ遷移
	if (m_ability.hp <= 0) {
		SetState(m_stateMap[TO_INT(IEnemy::EnState::enDeadState)]);
	}
}

void Boss_Fatman::Terminate()
{
	DeleteGO(m_model);
}
