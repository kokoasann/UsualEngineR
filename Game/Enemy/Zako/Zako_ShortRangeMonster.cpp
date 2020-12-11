#include "stdafx.h"
#include "Zako_ShortRangeMonster.h"

#include "Enemy/State/EnemyShortRangeStunState.h"
#include "Enemy/State/EnemyDeadState.h"
#include "Enemy/State/EnemyShortRangeIdleState.h"
#include "Enemy/State/EnemyShortRangeComingState.h"
#include "Enemy/State/EnemyShortRangePunchState.h"
#include "Enemy/State/EnemyShortRangeStrongPunchState.h"
#include "Enemy/State/EnemyShortRangeDanceState.h"


Zako_ShortRangeMonster::Zako_ShortRangeMonster()
{

}

Zako_ShortRangeMonster::~Zako_ShortRangeMonster()
{

}

void Zako_ShortRangeMonster::Init()
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

	/*auto ske = m_model->GetModel().GetSkelton();
	IK* headik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Head_IK")), 1, 1);
	headik->SetIKMode(IK::enMode_NoneHit);
	auto coming = reinterpret_cast<EnemyShortRangeComingState*>(GetState(TO_INT(EnStateEX::enComing)));
	coming->SetHeadIK(headik);*/

	InitAnimation();
	
	//State
	SetState(m_stateMap[TO_INT(IEnemy::EnState::enIdleState)]);

	//Physics
	InitCharacon(m_radius, m_height, m_position, true);
}

void Zako_ShortRangeMonster::InitAnimation()
{
	//Idle
	SetAnimation(TO_INT(IEnemy::EnAnimation::enIdle), "Assets/modelData/enemy/ShortRangeMonster/anim/srm_idle.tka", true);
	//Walk
	SetAnimation(TO_INT(IEnemy::EnAnimation::enWalk), "Assets/modelData/enemy/ShortRangeMonster/anim/srm_walk.tka", true);
	//Punch
	SetAnimation(TO_INT(IEnemy::EnAnimation::enAttackA), "Assets/modelData/enemy/ShortRangeMonster/anim/srm_punch.tka", false);
	//StrongPunch
	SetAnimation(TO_INT(IEnemy::EnAnimation::enAttackB), "Assets/modelData/enemy/ShortRangeMonster/anim/srm_strong_punch.tka", false);
	//Dance
	SetAnimation(TO_INT(EnAnimEX::enDance), "Assets/modelData/enemy/ShortRangeMonster/anim/srm_dance.tka", false);

	m_model->InitAnimation(m_animationMap, m_animationMap.size());
}

void Zako_ShortRangeMonster::InitState()
{
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enDeadState), new EnemyDeadState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enIdleState), new EnemyShortRangeIdleState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(EnStateEX::enComing), new EnemyShortRangeComingState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enAttackA), new EnemyShortRangePunchState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enAttackB), new EnemyShortRangeStrongPunchState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(EnStateEX::enDance), new EnemyShortRangeDanceState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(EnState::enStunState), new EnemyShortRangeStunState());
		m_stateMap.insert(p);
	}
}

void Zako_ShortRangeMonster::InitIK()
{
	auto ske = m_model->GetModel().GetSkelton();
	{
		IK* headik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Head_IK")), 1, 1);
		headik->SetIKMode(IK::enMode_NoneHit);

		SetIK(TO_INT(EnIK::enHead), headik);
	}
	{
		IK* ik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Bone.002")), 2, 1);
		ik->SetIKMode(IK::enMode_NoneHit);

		SetIK(TO_INT(EnIK::enChest), ik);
	}
}

void Zako_ShortRangeMonster::Execute()
{
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);

	//体力がなくなったら死亡ステートへ遷移
	if (m_ability.hp <= 0) {
		SetState(m_stateMap[TO_INT(IEnemy::EnState::enDeadState)]);
	}
}

void Zako_ShortRangeMonster::Terminate()
{
	DeleteGO(m_model);
}
