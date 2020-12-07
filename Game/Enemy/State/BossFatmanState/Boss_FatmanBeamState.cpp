#include "stdafx.h"
#include "Boss_FatmanBeamState.h"
#include "Enemy/IEnemy.h"
#include "GameManager.h"

Boss_FatmanBeamState::Boss_FatmanBeamState()
{
}

Boss_FatmanBeamState::~Boss_FatmanBeamState()
{
}

void Boss_FatmanBeamState::Enter(IEnemy* e)
{
	auto& p = GameManager::GetInstance().m_player;
	m_position = p->GetPosition();
}

IEnemyState* Boss_FatmanBeamState::Update(IEnemy* e)
{
	if (CrossJudge(e)) {
		//m_damageTimer += gameTime()->GetDeltaTime();
		auto& p = GameManager::GetInstance().m_player;
		p->ApplyDamage(m_damage);
	}
	return this;
}

void Boss_FatmanBeamState::Exit(IEnemy* e)
{
}

bool Boss_FatmanBeamState::CrossJudge(IEnemy* e)
{
	const float beamWidth = 30.0f;
	auto& epos = e->GetPosition();

	//敵からプレイヤーに向かうベクトル。
	Vector3 VecPtoE = m_position - epos;

	//外積。横方向に伸びたVecPtoEに直行するベクトル。
	Vector3 ECross;
	ECross.Cross(VecPtoE, Vector3::Up);
	ECross.Normalize();

	auto& p = GameManager::GetInstance().m_player;
	auto& ppos = p->GetPosition();
	Vector3 VecNewPtoE = ppos - epos;
	
	float dir = ECross.Dot(VecNewPtoE);

	if (std::abs(dir) < beamWidth) {
		return true;
	}
	return false;
}
