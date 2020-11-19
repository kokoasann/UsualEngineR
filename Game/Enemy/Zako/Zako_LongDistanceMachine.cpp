#include "stdafx.h"
#include "Zako_LongDistanceMachine.h"

#include "Enemy/State/EnemyLongDistanceAttackState.h"

Zako_LongDistanceMachine::Zako_LongDistanceMachine()
{
	m_stateListEX[TO_INT8(EStateEX::LongDistanceAttack)] = new EnemyLongDistanceAttackState();
}

Zako_LongDistanceMachine::~Zako_LongDistanceMachine()
{
	delete m_stateListEX[TO_INT8(EStateEX::LongDistanceAttack)];
}

void Zako_LongDistanceMachine::Init()
{
	InitState();
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
	m_animlist.resize(1);
	m_animlist[0] = std::make_unique<CAnimationClip>();
	m_animlist[0]->Load("Assets/modelData/enemy/LongRangeMachine/anim/lrm_walk.tka");
	m_animlist[0]->BuildKeyFramesAndAnimationEvents();
	m_animlist[0]->SetLoopFlag(true);

	m_model->InitAnimation(m_animlist, 1);
	m_model->Play(0);

	//State
	//SetState(m_stateList[static_cast<int>(IEnemy::EnState::enIdleState)]);
	SetState(m_stateListEX[TO_INT8(EStateEX::LongDistanceAttack)]);
	//Physics
	InitCharacon(m_radius, m_height, m_position, true);
}

void Zako_LongDistanceMachine::Execute()
{
	m_model->SetPosition(m_position);
}

void Zako_LongDistanceMachine::Terminate()
{
	DeleteGO(m_model);
}