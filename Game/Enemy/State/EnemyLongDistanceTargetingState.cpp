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
static ModelRender* mr;
void EnemyLongDistanceTargetingState::Init(IK* ik)
{
	m_ik = ik;
	mr = NewGO<ModelRender>(0);
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_upAxis = enUpAxisY;
	mr->Init(mid);
}

void EnemyLongDistanceTargetingState::Enter(IEnemy* e)
{
	m_timer = 0.f;
}

IEnemyState* EnemyLongDistanceTargetingState::Update(IEnemy* e)
{
	const float dtime = gameTime()->GetDeltaTime();

	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	auto pvel = player->GetVelocity();
	float pvelLen = pvel.Length();
	pvel.Normalize();

	const auto& epos = e->GetPosition();
	const auto& ppos = player->GetPosition();
	auto e2p = (ppos - epos);
	float e2pLen = e2p.Length();
	e2p.Normalize();

	auto tpos = ppos + (pvel * ( dtime*2));
	

	float y = tpos.y - epos.y;
	if (y < 0)
	{
		tpos.y = epos.y;
	}
	Bone* ebone = m_ik->GetEffectorBone();
	ebone->GetWorldMatrix().GetTransrate();

	mr->SetPosition(tpos);
	//Quaternion rot;
	//rot.SetRotation(epos, ppos);
	m_ik->SetNextTarget(tpos);

	m_timer += dtime;
	if (m_timer >= m_timeLimit)
	{
		auto v = tpos - epos;
		v.Normalize();
		auto st = reinterpret_cast<EnemyLongDistanceAttackState*>(e->GetState(TO_INT(Zako_LongDistanceMachine::EStateEX::LongDistanceAttack)));
		st->SetVelocity(v);
		return st;
	}
	
	return this;
}

void EnemyLongDistanceTargetingState::Exit(IEnemy* e)
{
}
