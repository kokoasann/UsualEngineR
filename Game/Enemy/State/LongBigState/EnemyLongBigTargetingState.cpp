#include "stdafx.h"
#include "EnemyLongBigTargetingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Zako/Zako_LongBig.h"
#include "Player/Player.h"
#include "GameManager.h"

EnemyLongBigTargetingState::EnemyLongBigTargetingState()
{
}

EnemyLongBigTargetingState::~EnemyLongBigTargetingState()
{
}

void EnemyLongBigTargetingState::Init(IK* ik, Quaternion& rot, float bulletSpeed)
{
	m_ik = ik;
	m_rot = &rot;

	//m_lda.SetIK(ik);
	m_speed = bulletSpeed;
	m_lba.Init(ik, bulletSpeed, this);
}

void EnemyLongBigTargetingState::Enter(IEnemy* e)
{
	m_timer = m_timeLimit;
	e->PlayAnimation(TO_INT(IEnemy::EnAnimation::enWalk));
	auto model = e->GetModel();
	((ModelRender*)model)->SetAnimPlaySpeed(3.f);
}

IEnemyState* EnemyLongBigTargetingState::Update(IEnemy* e)
{

	const float dtime = gameTime()->GetDeltaTime();

	auto player = GameManager::GetInstance().m_player;

	auto pvel = player->GetVelocity();
	float pvelLen = pvel.Length();
	pvel.Normalize();

	auto epos = e->GetPosition();
	epos.y += 5.f;
	const auto& ppos = player->GetPosition();
	auto e2p = (ppos - epos);
	float e2pLen = e2p.Length();
	e2p.Normalize();

	if (m_overLookRange < e2pLen)
	{
		return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
	}

	float bSpeed = m_speed * dtime;


	Vector3 tpos;
	float fn = 3;
	if (pvelLen > FLT_EPSILON)
	{
		pvelLen *= dtime;
		float cta = Dot(e2p, pvel);
		if (cta < 0)
		{
			fn = floorf(e2pLen / bSpeed);
		}
		else
		{

			//fn = floorf()
		}
		while (1)
		{
			auto target = (ppos + pvel * (pvelLen * fn));
			auto t = epos - target;
			//auto v = t;
			//v.Normalize();
			if ((bSpeed * (fn - 3.f)) >= t.Length())
				//if (bSpeed * dtime >= (epos + (v * bSpeed * (fn) * dtime) - target).Length())
			{
				/*DebugPrintValue(EDebugConsoloUser::NOMOTO, "e2pLen", e2pLen);
				DebugPrintValue(EDebugConsoloUser::NOMOTO, "pvelLen", pvelLen);
				DebugPrintValue(EDebugConsoloUser::NOMOTO, "fn", fn);
				auto c = Math::RadToDeg(acos(e2p.Dot(pvel)));
				DebugPrintValue(EDebugConsoloUser::NOMOTO, "deg", c);*/

				tpos = std::move(target);
				break;
			}
			fn += 1.f;
			if (fn > 20.f)
				break;
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

	//auto e2t = tpos - epos;
	float t = atan2f(tpos.x - epos.x, tpos.z - epos.z);
	Quaternion rot;
	m_rot->SetRotationY(t);

	m_timer += dtime;
	if (m_timer >= m_timeLimit)
	{
		//auto v = tpos - epos;
		//v.Normalize();

		m_lba.SetTarget(std::move(tpos));
		return &m_lba;
	}

	return this;
}

void EnemyLongBigTargetingState::Exit(IEnemy* e)
{
}
