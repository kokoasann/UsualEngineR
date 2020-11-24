#include "stdafx.h"
#include "EnemyLongDistanceTargetingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Zako/Zako_LongDistanceMachine.h"
#include "EnemyLongDistanceAttackState.h"
#include "Player/Player.h"

EnemyLongDistanceTargetingState::EnemyLongDistanceTargetingState()
{
}

EnemyLongDistanceTargetingState::~EnemyLongDistanceTargetingState()
{
}
void EnemyLongDistanceTargetingState::Init(IK* ik, Quaternion& rot)
{
	m_ik = ik;
	m_rot = &rot;
}

void EnemyLongDistanceTargetingState::Enter(IEnemy* e)
{
	m_timer = 0.f;
	e->PlayAnimation(TO_INT(Zako_LongDistanceMachine::EAnim::Walk));
	auto model = e->GetMode();
	((ModelRender*)model)->SetAnimPlaySpeed(3.f);
}

IEnemyState* EnemyLongDistanceTargetingState::Update(IEnemy* e)
{
	const float dtime = gameTime()->GetDeltaTime();

	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	auto pvel = player->GetVelocity();
	float pvelLen = pvel.Length();
	pvel.Normalize();

	auto epos = e->GetPosition();
	epos.y += 5.f;
	const auto& ppos = player->GetPosition();
	auto e2p = (ppos - epos);
	float e2pLen = e2p.Length();
	e2p.Normalize();

	float bSpeed = 500.f;


	auto tpos = ppos + (pvel * (fabsf(Dot(e2p,pvel)) * pvelLen * e2pLen * dtime * 5.f / bSpeed));
	float fn = 1;
	while (1)
	{
		auto t = epos - (ppos + pvel * (pvelLen * fn * dtime));
		if (bSpeed * fn * dtime)
		{
			tpos = t;

			break;
		}
	}

	

	float y = tpos.y - epos.y;
	if (y < 0)
	{
		//tpos.y = epos.y;
	}
	Bone* ebone = m_ik->GetEffectorBone();
	ebone->GetWorldMatrix().GetTransrate();

	//Quaternion rot;
	//rot.SetRotation(epos, ppos);
	m_ik->SetNextTarget(tpos);

	auto e2t = tpos - epos;
	float t = atan2f(e2t.x, e2t.z);
	Quaternion rot;
	m_rot->SetRotationY(t);

	m_timer += dtime;
	if (m_timer >= m_timeLimit)
	{
		//auto v = tpos - epos;
		//v.Normalize();
		auto st = reinterpret_cast<EnemyLongDistanceAttackState*>(e->GetState(TO_INT(Zako_LongDistanceMachine::EStateEX::LongDistanceAttack)));
		st->SetTarget(std::move(tpos));
		return st;
	}
	
	return this;
}

void EnemyLongDistanceTargetingState::Exit(IEnemy* e)
{
}
