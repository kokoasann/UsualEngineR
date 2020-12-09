#include "stdafx.h"
#include "EnemyShortRangeComingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Zako/Zako_ShortRangeMonster.h"
#include "GameManager.h"

EnemyShortRangeComingState::EnemyShortRangeComingState()
{
}

EnemyShortRangeComingState::~EnemyShortRangeComingState()
{
}

void EnemyShortRangeComingState::Enter(IEnemy* e)
{
	e->PlayAnimation(IEnemy::EnAnimation::enWalk);

	if (m_headIK == nullptr)
	{
		m_headIK = e->GetIK(TO_INT(IEnemy::EnIK::enHead));
	}
}

IEnemyState* EnemyShortRangeComingState::Update(IEnemy* e)
{
	auto player = GameManager::GetInstance().m_player;
	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;

	//IK
	if (m_headIK != nullptr)
	{
		auto p = ppos;
		p.y += 10;
		m_headIK->SetNextTarget(p);
	}

	//プレイヤーが空中にいるなら動かない。
	const float airPlayerYPos = 70.0f;
	if (ppos.y > airPlayerYPos) {
		return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
	}

	const float moveRange = 10.f;
	const float attackRange = 5.f;
	const float idleRange = 200.f;

	if (player->GetCurrentHP() > 0.f) {
		//近づいたら行動。
		if (vecToPlayer.Length() < attackRange) {
			auto rand = GRandom().Rand();
			//約30%の確率で行動変化。
			if (rand < 0.1f) {
				//ダンス。
				return e->GetState(TO_INT(Zako_ShortRangeMonster::EnStateEX::enDance));
			}
			else if (rand < 0.7f) {
				//パンチ。
				return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
			}
			else {
				//大振りのパンチ。
				return e->GetState(TO_INT(IEnemy::EnState::enAttackB));
			}
		}
		//離れたら移動。
		else {
			Move(e);
		}

		//離れたら移動停止。
		if (vecToPlayer.Length() > idleRange) {
			return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
		}
	}

	//歩いている途中でたまに踊る。
	const float maxTime = 2.5f;
	m_danceTimer += gameTime()->GetDeltaTime();
	if (m_danceTimer > maxTime) {
		auto rand = GRandom().Rand();
		//約15%の確率で踊る。
		if (rand < 0.15f) {
			m_danceTimer = 0.f;
			//ダンス。
			return e->GetState(TO_INT(Zako_ShortRangeMonster::EnStateEX::enDance));	
		}
		m_danceTimer = 0.f;
	}

	return this;
}

void EnemyShortRangeComingState::Exit(IEnemy* e)
{
	e->SetVelocity(Vector3::Zero);
}

void EnemyShortRangeComingState::Move(IEnemy* e)
{
	auto player = GameManager::GetInstance().GetPlayer();

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	vecToPlayer.Normalize();
	
	const float walkSpeed = 60.0f;
	Vector3 moveSpeed = vecToPlayer * walkSpeed;

	//重力。
	moveSpeed.y -= 100.0f;
	
	e->SetVelocity(moveSpeed);
}

bool EnemyShortRangeComingState::Animation()
{
	return true;
}
