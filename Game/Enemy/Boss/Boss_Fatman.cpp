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
	mid.m_tkmFilePath = "Assets/modelData/enemy/ShortRangeMonster/srm.tkm";
	mid.m_tksFilePath = "Assets/modelData/enemy/ShortRangeMonster/srm.tks";
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_upAxis = enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(Vector3::One * m_scale);

	//State
	SetState(m_stateMap[TO_INT(IEnemy::EnState::enIdleState)]);

	//Physics
	InitCharacon(m_radius, m_height, m_position, true);
}
void Boss_Fatman::InitState()
{
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enDeadState), new EnemyDeadState());
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
