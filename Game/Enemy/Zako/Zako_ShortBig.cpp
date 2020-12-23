#include "stdafx.h"
#include "Zako_ShortBig.h"
#include "Enemy/State/IEnemyState.h"
#include "Enemy/State/ShortBigState/EnemyShortBigIdle.h"
#include "Enemy/State/ShortBigState/EnemyShortBigStun.h"
#include "Enemy/State/EnemyDeadState.h"
#include "Enemy/State/ShortBigState/EnemyShortBigBlown.h"
#include "Enemy/State/ShortBigState/EnemyShortBigComing.h"
#include "Enemy/State/ShortBigState/EnemyShortBigDance.h"
#include "Enemy/State/ShortBigState/EnemyShortBigPunch.h"
#include "Enemy/State/ShortBigState/EnemyShortBigStrongPunch.h"

Zako_ShortBig::Zako_ShortBig()
{

}

Zako_ShortBig::~Zako_ShortBig()
{

}

void Zako_ShortBig::Init()
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

	InitAnimation();

	//State
	SetState(m_stateMap[static_cast<int>(IEnemy::EnState::enIdleState)]);

	//Physics
	InitCharacon(m_radius, m_height, m_position, true);
}

void Zako_ShortBig::InitAnimation()
{
	//�ЂƂ܂��A�ߋ����G���̃A�j���[�V�����𗬗p
	//Idle
	SetAnimation(TO_INT(IEnemy::EnAnimation::enIdle), "Assets/modelData/enemy/ShortRangeMonster/anim/srm_idle.tka", true);
	//Walk
	SetAnimation(TO_INT(IEnemy::EnAnimation::enWalk), "Assets/modelData/enemy/ShortRangeMonster/anim/srm_walk.tka", true);
	//run
	SetAnimation(TO_INT(IEnemy::EnAnimation::enRun), "Assets/modelData/enemy/ShortRangeMonster/anim/srm_run.tka", true);
	//Punch
	SetAnimation(TO_INT(IEnemy::EnAnimation::enAttackA), "Assets/modelData/enemy/ShortRangeMonster/anim/srm_punch.tka", false);
	//StrongPunch
	SetAnimation(TO_INT(IEnemy::EnAnimation::enAttackB), "Assets/modelData/enemy/ShortRangeMonster/anim/srm_strong_punch.tka", false);
	//Dance
	SetAnimation(TO_INT(EnAnimEX::enDance), "Assets/modelData/enemy/ShortRangeMonster/anim/srm_dance.tka", false);
	//Down
	SetAnimation(TO_INT(EnAnimEX::enDown), "Assets/modelData/enemy/ShortRangeMonster/anim/srm_down.tka", false);

	m_model->InitAnimation(m_animationMap, m_animationMap.size());
}
void Zako_ShortBig::InitState()
{
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enIdleState), new EnemyShortBigIdle());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enDeadState), new EnemyShortBigBlown());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(EnStateEX::enComing), new EnemyShortBigComing());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enAttackA), new EnemyShortBigPunch());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enAttackB), new EnemyShortBigStrongPunch());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(EnStateEX::enDance), new EnemyShortBigDance());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(EnState::enStunState), new EnemyShortBigStun());
		m_stateMap.insert(p);
	}
	//m_stateMap.insert(std::make_pair(TO_INT(EnState::enIdleState), new EnemyShortBigIdle()));
	//m_stateMap.insert(std::make_pair(TO_INT(EnState::enDeadState), new EnemyDeadState()));
}
void Zako_ShortBig::InitIK()
{

}

void Zako_ShortBig::Execute()
{
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
	//�̗͂��Ȃ��Ȃ����玀�S�X�e�[�g�֑J��
	if (m_ability.hp <= 0) {
		SetState(m_stateMap[TO_INT(IEnemy::EnState::enDeadState)]);
	}
}

void Zako_ShortBig::Terminate()
{
	DeleteGO(m_model);
}
