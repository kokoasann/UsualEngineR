#include "stdafx.h"
#include "BossA.h"

void BossA::Init() {

	//Model
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/boss/sp/sp.tkm";
	mid.m_tksFilePath = "Assets/modelData/boss/sp/sp.tks";
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_upAxis = EUpAxis::enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(m_scale);

	//anim
	const int numAnim = static_cast<int>(EnAnimation::enNumAnim);
	m_animlist.resize(numAnim);

	//Idle
	m_animlist[static_cast<int>(EnAnimation::enIdle)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enIdle)]->Load("Assets/modelData/boss/sp/anim/sp_idle.tka");
	m_animlist[static_cast<int>(EnAnimation::enIdle)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enIdle)]->SetLoopFlag(true);

	//Walk
	m_animlist[static_cast<int>(EnAnimation::enWalk)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enWalk)]->Load("Assets/modelData/boss/sp/anim/sp_idle.tka");
	m_animlist[static_cast<int>(EnAnimation::enWalk)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enWalk)]->SetLoopFlag(true);

	//Run
	m_animlist[static_cast<int>(EnAnimation::enRun)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enRun)]->Load("Assets/modelData/boss/sp/anim/sp_dush.tka");
	m_animlist[static_cast<int>(EnAnimation::enRun)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enRun)]->SetLoopFlag(true);

	//AttackA
	m_animlist[static_cast<int>(EnAnimation::enAttackA)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enAttackA)]->Load("Assets/modelData/boss/sp/anim/sp_punch.tka");
	m_animlist[static_cast<int>(EnAnimation::enAttackA)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enAttackA)]->SetLoopFlag(false);

	//AttackB
	m_animlist[static_cast<int>(EnAnimation::enAttackB)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enAttackB)]->Load("Assets/modelData/boss/sp/anim/sp_punch_strong.tka");
	m_animlist[static_cast<int>(EnAnimation::enAttackB)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enAttackB)]->SetLoopFlag(false);

	m_model->InitAnimation(m_animlist, m_animlist.size());
	PlayAnimation(EnAnimation::enIdle);

	//Init State
	SetState(m_stateList[static_cast<int>(IEnemy::EnState::enIdleState)]);
	m_isDrawHpBarAboveMyself = false;

	//Physics
	InitCharacon(m_radius,m_height, m_position, true);
}

void BossA::Terminate() {
	DeleteGO(m_model);
}

void BossA::Execute() {
	m_model->SetPosition(m_position);
}