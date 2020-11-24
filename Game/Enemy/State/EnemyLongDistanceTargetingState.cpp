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


	Vector3 tpos;
	float fn = 1;
	if (pvelLen > FLT_EPSILON)
	{
		while (1)
		{
			auto target = (ppos + pvel * (pvelLen * fn * dtime));
			auto t = epos - target;
			auto v = t;
			v.Normalize();
			if ((bSpeed * (fn - 3.f) * dtime) >= t.Length())
			//if (bSpeed * dtime >= (epos + (v * bSpeed * (fn) * dtime) - target).Length())
			{
				DebugPrintValue(EDebugConsoloUser::NOMOTO, "e2pLen", e2pLen);
				DebugPrintValue(EDebugConsoloUser::NOMOTO, "pvelLen", pvelLen);
				DebugPrintValue(EDebugConsoloUser::NOMOTO, "fn", fn);
				auto c = Math::RadToDeg(acos(e2p.Dot(pvel)));
				DebugPrintValue(EDebugConsoloUser::NOMOTO, "deg", c);

				tpos = std::move(target);
				break;
			}
			fn += 1.f;
		}
	}
	else
	{
		tpos = ppos;
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
