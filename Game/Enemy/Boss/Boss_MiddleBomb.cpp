#include "stdafx.h"
#include "Boss_MiddleBomb.h"
#include "Enemy/Drop/DropItemBom.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/State/MiddleBombState/BossBombIdleState.h"
#include "Enemy/State/MiddleBombState/BossBombDeadState.h"
#include "Enemy/State/MiddleBombState/BossBombBattleState.h"
#include "Enemy/State/MiddleBombState/BossBombLaunchState.h"
#include "Enemy/State/MiddleBombState/BossBombJumpState.h"
#include "Enemy/State/MiddleBombState/BossBombGuardState.h"
#include "Enemy/State/MiddleBombState/BossBombCoroCoroState.h"
#include "Enemy/State/MiddleBombState/BossBombFullFrontalState.h"
#include "Enemy/State/MiddleBombState/BossBombBashState.h"
#include "Enemy/State/MiddleBombState/BossBombRollingState.h"
#include "Enemy/State/MiddleBombState/BossBombStunState.h"
#include "Enemy/BossBGM.h"

Boss_MiddleBomb::Boss_MiddleBomb()
{

}

Boss_MiddleBomb::~Boss_MiddleBomb()
{
	Physics().RemoveRigidBody(m_rigidBody);
	m_shieldGhost.Release();
}

void Boss_MiddleBomb::Init()
{
	m_isNormalEnemy = false;

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
	m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Wall | GameCollisionAttribute::BombShield);
	m_rigidBody.GetBody()->setUserPointer((void*)1999);
	Physics().AddRigidBody(m_rigidBody);

	Matrix offsetScaleMat;
	offsetScaleMat.MakeScaling({ 1.3f,2.f,1.f });
	m_meshColl_ghost.CreateFromSkinModel(model, &offsetScaleMat);
	m_shieldGhost.Create(&m_meshColl_ghost, { 0,0,0 }, Quaternion::Identity);


	SetAutoRotateFlag(false);

	InitAnim();

	{
		auto ske = m_model->GetModel().GetSkelton();
		m_shieldBone = ske->GetBone(ske->FindBoneID(L"Shield"));
		auto ik = m_model->CreateIK(m_shieldBone, 1, 0.5f);
		SetIK(TO_INT(EnIK::enArm_R), ik);
		ik->SetIKMode(IK::enMode_NoneHit);
	}

	//State
	SetState(m_stateMap[static_cast<int>(IEnemy::EnState::enIdleState)]);

	//Physics
	InitCharacon(m_radius, m_height, m_position, true);
	m_charaCon.AddNonHitCollisionAttribute(TO_INT(GameCollisionAttribute::BombShield));
	m_isNormalEnemy = false;

	BossBombData::GetInstance().meshColl = &m_meshColl;
	BossBombData::GetInstance().rigidBody = &m_rigidBody;
	BossBombData::GetInstance().shieldGhost = &m_shieldGhost;

	GetModel()->GetAnimation().AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName)
		{
			int res = std::wcscmp(eventName, L"jump_start");
			if (res == 0)
			{
				BossBombData::GetInstance().isJumpStart = true;
				return;
			}
			else
				BossBombData::GetInstance().isJumpStart = false;


			res = std::wcscmp(eventName, L"bash_start");
			if (res == 0)
			{
				printf("start\n");
				BossBombData::GetInstance().isBashAttack = true;
				return;
			}
			res = std::wcscmp(eventName, L"bash_end");
			if (res == 0)
			{
				printf("end\n");
				BossBombData::GetInstance().isBashAttack = false;
				return;
			}
		});
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
		auto s = std::make_pair(TO_INT(EnStateEX::Rolling), new BossBombRollingState());
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
	{
		auto s = std::make_pair(TO_INT(EnState::enStunState), new BossBombStunState());
		m_stateMap.insert(s);
	}
}
void Boss_MiddleBomb::InitIK()
{
	auto ske = m_model->GetModel().GetSkelton();
	m_shieldBone = ske->GetBone(ske->FindBoneID(L"Shield"));

	{
		auto ik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Head_IK")), 1, 0.5f);
		SetIK(TO_INT(EnIK::enHead), ik);
		ik->SetIKMode(IK::enMode_NoneHit);
	}
	{
		auto ik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Cannon_IK")), 2, 0.5f);
		SetIK(TO_INT(EnIK::enArm_L), ik);
		ik->SetIKMode(IK::enMode_NoneHit);
	}
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
	SetAnimation(TO_INT(EnAnimEX::Angry), "Assets/modelData/boss/mb/anim/mb_angry.tka", false);

	m_model->InitAnimation(m_animationMap, TO_INT(EnAnimEX::Num));
	m_model->Play(TO_INT(IEnemy::EnAnimation::enIdle));
}

void Boss_MiddleBomb::ExecuteCommon()
{
	const auto& wm = m_shieldBone->GetWorldMatrix();
	auto pos = wm.GetTransrate();
	auto rot = wm.GetRotate();

	m_ShieldModel->SetPosition(pos);
	m_ShieldModel->SetRotation(rot);

	m_shieldGhost.SetPosition(pos);
	m_shieldGhost.SetRotation(rot);

	auto& tra = m_rigidBody.GetBody()->getWorldTransform();
	tra.setOrigin({ pos.x, pos.y, pos.z });
	tra.setRotation({ rot.x ,rot.y ,rot.z ,rot.w });
}

void Boss_MiddleBomb::Execute()
{
	const float hpper = m_ability.hp / m_ability.max_hp;
	if(hpper < 1.f / 5.f)
	{
		BossBombData::GetInstance().feeling = BossBombData::EnFeel::Tired;
	}
	else if (hpper < 2.f / 3.f)
	{
		BossBombData::GetInstance().feeling = BossBombData::EnFeel::Angry;
	}

	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);

	/*btTransform tra;
	m_rigidBody.GetBody()->getMotionState()->getWorldTransform(tra);
	tra.setOrigin({ pos.x, pos.y, pos.z });
	tra.setRotation({rot.x ,rot.y ,rot.z ,rot.w });
	m_rigidBody.GetBody()->getMotionState()->setWorldTransform(tra);*/
	//m_rigidBody.GetBody()->setWorldTransform(tra);
}

void Boss_MiddleBomb::Terminate()
{
	DeleteGO(m_model);
	DeleteGO(m_ShieldModel);
	EnemyManager::GetEnemyManager().SetBombBoss(nullptr);
}

void Boss_MiddleBomb::SpawnDropItem() {
	auto item = NewGO<DropItemBom>(0);
	item->SetPosition(m_position);
}
