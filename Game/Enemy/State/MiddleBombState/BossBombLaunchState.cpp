#include "stdafx.h"
#include "BossBombLaunchState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Equipment/Enemy_LaunchBomb.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "GameManager.h"

#include "Effect/MuzzleFlash.h"

BossBombLaunchState::BossBombLaunchState():
	m_damage(Player::GetMaxHP()/15.f)
{
	m_effect = NewGO<MuzzleFlash>(0);
	MuzzleFlashEffectInitData eid;
	
	m_effect->Init(eid);
}

BossBombLaunchState::~BossBombLaunchState()
{
}

void BossBombLaunchState::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(IEnemy::EnAnimation::enAttackA));

	auto ik = e->GetIK(TO_INT(IEnemy::EnIK::enArm_L));
	auto bone = ik->GetEffectorBone();
	m_cannonBone = e->GetModel()->GetModel().GetSkelton()->GetBone(bone->GetParentBoneNo());
	e->SetAutoRotateFlag(false);

	m_firstRot = e->GetRotation();
	m_timer = 0.f;
}

IEnemyState* BossBombLaunchState::Update(IEnemy* e)
{
	m_timer += gameTime()->GetDeltaTime();
	float t = m_timer / m_timeLimit;
	t = min(t, 1.f);

	auto ik = e->GetIK(TO_INT(IEnemy::EnIK::enArm_L));
	auto bone = ik->GetEffectorBone();

	ik->SetNextTarget(GameManager::GetInstance().GetPlayer()->GetPosition());
	
	auto e2p = GameManager::GetInstance().GetPlayer()->GetPosition() - e->GetPosition();
	e2p.y = 0.;
	e2p.Normalize();
	//Quaternion rot(Vector3::Front, e2p);
	{
		Quaternion rot;
		auto theta = atan2(e2p.x, e2p.z);
		//theta = theta * (180.f / Math::PI);
		rot.SetRotationY(theta);
		rot.Slerp(t, m_firstRot, rot);
		e->SetRotation(rot);
	}


	if (e->GetModel()->IsAnimPlaying())
		return this;

	
	const auto& mat = bone->GetWorldMatrix();
	const auto& pmat = m_cannonBone->GetWorldMatrix();

	Enemy_LaunchBomb* bomb = NewGO<Enemy_LaunchBomb>(0,true);
	Vector3 v = Vector3::Up;
	pmat.GetRotate().Apply(v);
	//v.y += 0.3;
	auto bonePos = mat.GetTransrate();
	bomb->Init(bonePos, v*500., 1,20, m_damage,25);
	CSoundSource* se = NewGO<CSoundSource>(0);
	se->Init(L"Assets/sound/shoot.wav");
	se->SetVolume(1.);
	se->Play(false);

	m_effect->Play();
	m_effect->SetPos(bonePos);
	m_effect->SetRot(pmat.GetRotate());

	return e->GetState(TO_INT(Boss_MiddleBomb::EnStateEX::Guard));
	//return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
}

void BossBombLaunchState::Exit(IEnemy* e)
{
}

void BossBombLaunchState::OnAttacked(IEnemy* e)
{
}
