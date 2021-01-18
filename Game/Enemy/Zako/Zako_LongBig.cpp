#include "stdafx.h"
#include "Zako_LongBig.h"
#include "Enemy/State/EnemyIdleState.h"
#include "Enemy/State/EnemyDeadState.h"
#include "Enemy/State/LongBigState/EnemyLongBigAttackState.h"
#include "Enemy/State/LongBigState/EnemyLongBigTargetingState.h"
#include "Enemy/State/LongBigState/EnemyLongBigBlownState.h"
#include "Enemy/State/EnemyShortRangeStunState.h"

Zako_LongBig::Zako_LongBig()
{

}

Zako_LongBig::~Zako_LongBig()
{
	//delete m_stateList[TO_UINT(EStateEX::LongDistanceAttack)];
	Physics().RemoveRigidBody(m_rigidBody);
}

void Zako_LongBig::Init()
{
	////Model
	//ModelInitData mid;
	//mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";
	//mid.m_vsEntryPointFunc = "VSMain";
	//mid.m_psEntryPointFunc = "PSMain";
	//mid.m_upAxis = enUpAxisY;

	//m_model = NewGO<ModelRender>(0);
	//m_model->Init(mid);
	//m_model->SetScale(Vector3::One * m_scale);

	////State
	//SetState(m_stateMap[static_cast<int>(IEnemy::EnState::enIdleState)]);

	////Physics
	//InitCharacon(m_radius, m_height, m_position, true);

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
	SetAnimation(TO_INT(EnAnimEX::Fire), "Assets/modelData/enemy/LongRangeMachine/anim/lrm_fire.tka", false);

	m_model->InitAnimation(m_animationMap, TO_INT(EnAnimEX::Num));
	m_model->Play(TO_INT(IEnemy::EnAnimation::enIdle));

	auto& model = m_model->GetModel();
	auto ske = model.GetSkelton();
	IK* ik = model.CreateIK(ske->GetBone(ske->FindBoneID(L"Bone.003")), 1, 0.5);
	ik->SetIKMode(IK::enMode_NoneHit);
	reinterpret_cast<EnemyLongBigTargetingState*>(m_stateMap[TO_UINT(EnStateEX::LongBigTargeting)])->Init(ik, m_rot, m_bulletSpeed);
	//reinterpret_cast<EnemyLongDistanceAttackState*>(m_stateList[TO_UINT(EStateEX::LongDistanceAttack)])->SetIK(ik);

	//State
	SetState(m_stateMap[static_cast<int>(IEnemy::EnState::enIdleState)]);
	//SetState(m_stateMap[TO_UINT(EStateEX::LongDistanceTargeting)]);
	//Physics
	InitCharacon(m_radius, m_height, m_position, true);

	m_box.Create({ 4,4,4 });
	RigidBodyInfo rbinfo;
	rbinfo.collider = &m_box;
	rbinfo.mass = 1;
	//static int i = 1;
	//rbinfo.pos = { 0,(float)(i*15),0 };
	//i++;
	m_rigidBody.Create(rbinfo);
	auto b = m_rigidBody.GetBody();
	b->setUserIndex(enCollisionAttr_Character | enCollisionAttr_NonHit);

	m_motionState = b->getMotionState();
	//btDefaultMotionState
	b->activate(false);

	//b->setCollisionFlags(btCollisionObject::CF_DISABLE_SPU_COLLISION_PROCESSING);
	b->setGravity({ 0,0,0 });
	Physics().AddRigidBody(m_rigidBody);
}
void Zako_LongBig::InitState()
{
	//m_stateMap.insert(std::make_pair(TO_INT(EnState::enIdleState), new EnemyIdleState()));
	//m_stateMap.insert(std::make_pair(TO_INT(EnState::enDeadState), new EnemyDeadState()));
	{
		/*auto p = std::make_pair(TO_INT(IEnemy::EnState::enDeadState), new EnemyDeadState());
		m_stateMap.insert(p);*/
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enDeadState), new EnemyLongBigBlownState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enIdleState), new EnemyIdleState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(EnStateEX::LongBigTargeting), new EnemyLongBigTargetingState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(EnState::enStunState), new EnemyShortRangeStunState());
		m_stateMap.insert(p);
	}

}
void Zako_LongBig::InitIK()
{
	auto ske = m_model->GetModel().GetSkelton();
	{
		IK* ik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Bone.003")), 1, 0.5);
		ik->SetIKMode(IK::enMode_NoneHit);
		SetIK(TO_INT(EnIK::enHead), ik);
		reinterpret_cast<EnemyLongBigTargetingState*>(m_stateMap[TO_UINT(EnStateEX::LongBigTargeting)])->Init(ik, m_rot, m_bulletSpeed);
	}
	{
		IK* ik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Bone")), 1, 3);
		//IK* ik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Bone.002")), 1, 3);
		ik->SetIKMode(IK::enMode_NoneHit);

		SetIK(TO_INT(EnIK::enChest), ik);
	}

	{
		IK* ik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Bone_L.008")), 2, 1);
		ik->SetIKMode(IK::enMode_NoneHit);

		SetIK(TO_INT(EIK::Foot1), ik);
	}
	{
		IK* ik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Bone_L.009")), 2, 1);
		ik->SetIKMode(IK::enMode_NoneHit);

		SetIK(TO_INT(EIK::Foot2), ik);
	}
	{
		IK* ik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Bone_R.008")), 2, 1);
		ik->SetIKMode(IK::enMode_NoneHit);

		SetIK(TO_INT(EIK::Foot3), ik);
	}
	{
		IK* ik = m_model->CreateIK(ske->GetBone(ske->FindBoneID(L"Bone_R.009")), 2, 1);
		ik->SetIKMode(IK::enMode_NoneHit);

		SetIK(TO_INT(EIK::Foot4), ik);
	}
}

void Zako_LongBig::Execute()
{
	if (!m_isKinematic)
	{
		m_model->SetPosition(m_position);
		m_model->SetRotation(m_rot);

		auto b = m_rigidBody.GetBody();
		auto& t = b->getWorldTransform();
		t.setOrigin({ m_position.x,m_position.y + 5.f,m_position.z });
		t.setRotation({ m_rot.x,m_rot.y,m_rot.z,m_rot.w });

		/*btTransform tra;
		m_motionState->getWorldTransform(tra);
		tra.setOrigin({ m_position.x,m_position.y+5.f ,m_position.z });
		m_motionState->setWorldTransform(tra);*/
	}
	else
	{
		auto b = m_rigidBody.GetBody();

		m_model->SetRotation(b->getWorldTransform().getRotation());
		m_rot = b->getWorldTransform().getRotation();

		auto up = Vector3::Up;
		m_rot.Apply(up);
		Vector3 pos = b->getWorldTransform().getOrigin();
		pos += up * -3;
		m_model->SetPosition(pos);
		m_position = pos;
	}

	//体力がなくなったら死亡ステートへ遷移
	if (m_ability.hp <= 0) {
		SetState(m_stateMap[static_cast<int>(IEnemy::EnState::enDeadState)]);
	}
}

void Zako_LongBig::Terminate()
{
	DeleteGO(m_model);
}
