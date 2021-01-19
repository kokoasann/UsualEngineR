#include "stdafx.h"
#include "Boss_MiddleBomb.h"

#include "Enemy/State/MiddleBombState/BossBombIdleState.h"
#include "Enemy/State/MiddleBombState/BossBombDeadState.h"
#include "Enemy/State/MiddleBombState/BossBombBattleState.h"
#include "Enemy/State/MiddleBombState/BossBombLaunchState.h"
#include "Enemy/State/MiddleBombState/BossBombJumpState.h"
#include "Enemy/State/MiddleBombState/BossBombGuardState.h"
#include "Enemy/State/MiddleBombState/BossBombCoroCoroState.h"
#include "Enemy/State/MiddleBombState/BossBombFullFrontalState.h"
#include "Enemy/State/MiddleBombState/BossBombBashState.h"


Boss_MiddleBomb::Boss_MiddleBomb()
{

}

Boss_MiddleBomb::~Boss_MiddleBomb()
{
	Physics().RemoveRigidBody(m_rigidBody);
}

void Boss_MiddleBomb::Init()
{
	//Model
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/boss/mb/mb.tkm";
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_upAxis = enUpAxisY;
	mid.m_tksFilePath = "Assets/modelData/boss/mb/mb.tks";

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(Vector3::One * m_scale);

	
	m_ShieldModel = NewGO<ModelRender>(0);
	mid.m_tkmFilePath = "Assets/modelData/boss/mb/mb_Shield.tkm";
	mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_tksFilePath = nullptr;
	mid.m_upAxis = enUpAxisZ;
	m_ShieldModel->Init(mid);
	m_ShieldModel->SetScale(Vector3::One * m_scale);

	Model model;
	mid.m_tkmFilePath = "Assets/modelData/boss/mb/mb_ShieldColli.tkm";
	mid.m_upAxis = enUpAxisZ;
	model.Init(mid);
	m_meshColl.CreateFromSkinModel(model);
	RigidBodyInfo info;
	info.collider = &m_meshColl;
	info.mass = 0;
	m_rigidBody.Create(info);
	m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Wall);
	Physics().AddRigidBody(m_rigidBody);

	InitAnim();


	//State
	SetState(m_stateMap[static_cast<int>(IEnemy::EnState::enIdleState)]);

	//Physics
	InitCharacon(m_radius, m_height, m_position, true);
}
void Boss_MiddleBomb::InitState()
{
	{
		auto s = std::make_pair(TO_INT(IEnemy::EnState::enIdleState), new BossBombIdleState());
		m_stateMap.insert(s);
	}
	{
		auto s = std::make_pair(TO_INT(IEnemy::EnState::enDeadState), new BossBombDeadState());
		m_stateMap.insert(s);
	}
	{
		auto s = std::make_pair(TO_INT(EnState::enBattleState), new BossBombBattleState());
		m_stateMap.insert(s);
	}
	{
		auto s = std::make_pair(TO_INT(EnStateEX::Luanch), new BossBombLaunchState());
		m_stateMap.insert(s);
	}
	{
		auto s = std::make_pair(TO_INT(EnStateEX::Corocoro), new BossBombCoroCoroState());
		m_stateMap.insert(s);
	}
	{
		auto s = std::make_pair(TO_INT(EnStateEX::FullFrontal), new BossBombFullFrontalState());
		m_stateMap.insert(s);
	}
	{
		auto s = std::make_pair(TO_INT(EnStateEX::Bash), new BossBombBashState());
		m_stateMap.insert(s);
	}
	{
		auto s = std::make_pair(TO_INT(EnStateEX::Guard), new BossBombGuardState());
		m_stateMap.insert(s);
	}
	{
		auto s = std::make_pair(TO_INT(EnStateEX::Jump), new BossBombJumpState());
		m_stateMap.insert(s);
	}
}
void Boss_MiddleBomb::InitIK()
{
	auto ske = m_model->GetModel().GetSkelton();
	m_shieldBone = ske->GetBone(ske->FindBoneID(L"Shield"));
}
void Boss_MiddleBomb::InitAnim()
{
	SetAnimation(TO_INT(IEnemy::EnAnimation::enIdle), "Assets/modelData/boss/mb/anim/mb_idle.tka", true);
	SetAnimation(TO_INT(EnAnimEX::Firing), "Assets/modelData/boss/mb/anim/mb_firing.tka", false);
	SetAnimation(TO_INT(EnAnimEX::FiringUp), "Assets/modelData/boss/mb/anim/mb_firing_up.tka", false);
	SetAnimation(TO_INT(EnAnimEX::FullFrontal), "Assets/modelData/boss/mb/anim/mb_fullfrontal_firing.tka", false);
	SetAnimation(TO_INT(EnAnimEX::Bash), "Assets/modelData/boss/mb/anim/mb_bash.tka", false);
	SetAnimation(TO_INT(EnAnimEX::Guard), "Assets/modelData/boss/mb/anim/mb_guard.tka", false);
	SetAnimation(TO_INT(EnAnimEX::Jump), "Assets/modelData/boss/mb/anim/mb_jump.tka", false);

	m_model->InitAnimation(m_animationMap, TO_INT(EnAnimEX::Num));
	m_model->Play(TO_INT(IEnemy::EnAnimation::enIdle));
}

void Boss_MiddleBomb::Execute()
{
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);

	const auto& wm = m_shieldBone->GetWorldMatrix();
	auto pos = wm.GetTransrate();
	auto rot = wm.GetRotate();

	m_ShieldModel->SetPosition(pos);
	m_ShieldModel->SetRotation(rot);

	/*btTransform tra;
	m_rigidBody.GetBody()->getMotionState()->getWorldTransform(tra);
	tra.setOrigin({ pos.x, pos.y, pos.z });
	tra.setRotation({rot.x ,rot.y ,rot.z ,rot.w });
	m_rigidBody.GetBody()->getMotionState()->setWorldTransform(tra);*/

	auto& tra = m_rigidBody.GetBody()->getWorldTransform();
	tra.setOrigin({ pos.x, pos.y, pos.z });
	tra.setRotation({ rot.x ,rot.y ,rot.z ,rot.w });
	//m_rigidBody.GetBody()->setWorldTransform(tra);
}

void Boss_MiddleBomb::Terminate()
{
	DeleteGO(m_model);
}

