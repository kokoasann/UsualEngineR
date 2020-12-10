#include "stdafx.h"
#include "Zako_LongDistanceMachine.h"

#include "Enemy/State/EnemyDeadState.h"
#include "Enemy/State/EnemyIdleState.h"
#include "Enemy/State/EnemyLongDistanceAttackState.h"
#include "Enemy/State/EnemyLongDistanceTargetingState.h"
#include "Enemy/State/EnemyShortRangeStunState.h"

Zako_LongDistanceMachine::Zako_LongDistanceMachine()
{
	
}

Zako_LongDistanceMachine::~Zako_LongDistanceMachine()
{
	//delete m_stateList[TO_UINT(EStateEX::LongDistanceAttack)];
}

void Zako_LongDistanceMachine::Init()
{
	//Model
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/enemy/LongRangeMachine/LongRangeMachine.tkm";
	mid.m_tksFilePath = "Assets/modelData/enemy/LongRangeMachine/LongRangeMachine.tks";
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_upAxis = enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(Vector3::One * m_scale);
	//m_model->SetMulColor({ 0.5, 0, 0.5, 1 });
	
	/*m_animlist.resize(TO_INT(EAnim::Num));
	m_animlist[TO_INT(EAnim::Idle)] = std::make_unique<CAnimationClip>();
	m_animlist[TO_INT(EAnim::Idle)]->Load("Assets/modelData/enemy/LongRangeMachine/anim/lrm_idol.tka");
	m_animlist[TO_INT(EAnim::Idle)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[TO_INT(EAnim::Idle)]->SetLoopFlag(true);*/
	
	/*m_animlist[TO_INT(EAnim::Walk)] = std::make_unique<CAnimationClip>();
	m_animlist[TO_INT(EAnim::Walk)]->Load("Assets/modelData/enemy/LongRangeMachine/anim/lrm_walk.tka");
	m_animlist[TO_INT(EAnim::Walk)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[TO_INT(EAnim::Walk)]->SetLoopFlag(true);

	m_animlist[TO_INT(EAnim::Fire)] = std::make_unique<CAnimationClip>();
	m_animlist[TO_INT(EAnim::Fire)]->Load("Assets/modelData/enemy/LongRangeMachine/anim/lrm_fire.tka");
	m_animlist[TO_INT(EAnim::Fire)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[TO_INT(EAnim::Fire)]->SetLoopFlag(false);*/

	SetAnimation(TO_INT(IEnemy::EnAnimation::enIdle), "Assets/modelData/enemy/LongRangeMachine/anim/lrm_idol.tka", true);
	SetAnimation(TO_INT(IEnemy::EnAnimation::enWalk), "Assets/modelData/enemy/LongRangeMachine/anim/lrm_walk.tka", true);
	SetAnimation(TO_INT(EAnim::Fire), "Assets/modelData/enemy/LongRangeMachine/anim/lrm_fire.tka", false);

	m_model->InitAnimation(m_animationMap, TO_INT(EAnim::Num));
	m_model->Play(TO_INT(IEnemy::EnAnimation::enIdle));

	auto& model = m_model->GetModel();
	auto ske = model.GetSkelton();
	IK* ik = model.CreateIK(ske->GetBone(ske->FindBoneID(L"Bone.003")), 1, 0.5);
	ik->SetIKMode(IK::enMode_NoneHit);
	reinterpret_cast<EnemyLongDistanceTargetingState*>(m_stateMap[TO_UINT(EStateEX::LongDistanceTargeting)])->Init(ik,m_rot,m_bulletSpeed);
	//reinterpret_cast<EnemyLongDistanceAttackState*>(m_stateList[TO_UINT(EStateEX::LongDistanceAttack)])->SetIK(ik);

	//State
	SetState(m_stateMap[static_cast<int>(IEnemy::EnState::enIdleState)]);
	//SetState(m_stateMap[TO_UINT(EStateEX::LongDistanceTargeting)]);
	//Physics
	InitCharacon(m_radius, m_height, m_position, true);
}

void Zako_LongDistanceMachine::InitState()
{
	/*m_stateList.resize(TO_INT(EStateEX::Num));
	m_stateList[TO_INT(EStateEX::Dead)] = new EnemyDeadState();
	m_stateList[TO_INT(EStateEX::Idle)] = new EnemyIdleState();
	m_stateList[TO_UINT(EStateEX::LongDistanceTargeting)] = new EnemyLongDistanceTargetingState();*/

	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enDeadState), new EnemyDeadState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enIdleState), new EnemyIdleState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(EStateEX::LongDistanceTargeting), new EnemyLongDistanceTargetingState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(EnState::enStunState), new EnemyShortRangeStunState());
		m_stateMap.insert(p);
	}
}

void Zako_LongDistanceMachine::InitIK()
{
	auto ske = m_model->GetModel().GetSkelton();
	{
		IK* ik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Bone")), 1, 1);
		ik->SetIKMode(IK::enMode_NoneHit);

		SetIK(TO_INT(EnIK::enChest), ik);
	}
}

void Zako_LongDistanceMachine::Execute()
{
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rot);

	//体力がなくなったら死亡ステートへ遷移
	if (m_ability.hp <= 0) {
		SetState(m_stateMap[static_cast<int>(IEnemy::EnState::enDeadState)]);
	}
}

void Zako_LongDistanceMachine::Terminate()
{
	DeleteGO(m_model);
}