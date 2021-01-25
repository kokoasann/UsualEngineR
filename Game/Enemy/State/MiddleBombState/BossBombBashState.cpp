#include "stdafx.h"
#include "BossBombBashState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "GameManager.h"

#include "Enemy/Equipment/Enemy_LaunchBomb.h"

BossBombBashState::BossBombBashState()
{
}

BossBombBashState::~BossBombBashState()
{
}

void BossBombBashState::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(Boss_MiddleBomb::EnAnimEX::Bash));
	m_timer = 0.;
}

IEnemyState* BossBombBashState::Update(IEnemy* e)
{
	m_timer += gameTime()->GetDeltaTime();
	if (m_timer > m_timeLimit)
	{
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}
	//Enemy_LaunchBomb* bomb = NewGO<Enemy_LaunchBomb>(0, true);
	//Vector3 v = e->GetPosition();
	//bomb->Init(v,v * 100, 1, 20, 1, 25);

	return this;
}

void BossBombBashState::Exit(IEnemy* e)
{
}

void BossBombBashState::OnAttacked(IEnemy* e)
{
}
