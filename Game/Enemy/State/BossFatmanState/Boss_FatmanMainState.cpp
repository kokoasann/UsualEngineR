#include "stdafx.h"
#include "Boss_FatmanMainState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_Fatman.h"

Boss_FatmanMainState::Boss_FatmanMainState()
{
}

Boss_FatmanMainState::~Boss_FatmanMainState()
{
}

void Boss_FatmanMainState::Enter(IEnemy* e)
{
	//重力。
	Vector3 gravity = Vector3::Zero;
	gravity.y = -5000.f;
	e->SetVelocity(gravity);
	
	m_fatTimer = 0.f;

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);
}
IEnemyState* Boss_FatmanMainState::Update(IEnemy* e)
{
	m_fatTimer += gameTime()->GetDeltaTime();

	float attackSpan = 0.9f;
	if (EnBattlePhase::Mad == Boss_Fatman::GetCurrentBattlePhase()) {
		attackSpan = 0.3f;
	}
	else if (EnBattlePhase::Tired == Boss_Fatman::GetCurrentBattlePhase()) {
		attackSpan = 0.6f;
	}

	if (m_fatTimer > attackSpan) {
		/*auto& p = GameManager::GetInstance().m_player;
		const auto& ppos = p->GetPosition();
		return e->GetState(TO_INT(IEnemy::EnState::enAttackA));*/

		auto player = GameManager::GetInstance().m_player;

		if (player->GetCurrentHP() > 0.f) {
			auto& epos = e->GetPosition();
			auto& ppos = player->GetPosition();
			auto vecToPlayer = ppos - epos;

			float diffY = ppos.y - epos.y;
			const float flyRange = 5.f;
			const float attackRange = 75.f;
			//離れたら遠距離攻撃。
			//もしくはプレイヤーが飛んでいたら遠距離攻撃。
			if (vecToPlayer.Length() > attackRange or diffY > flyRange) {
				return LongRangeAttack(e);
			}
			//近づいたらタックルか距離をとる。
			else {
				//確率で行動変化。
				auto rand = GRandom().Rand();
				if (rand < 0.5f) {
					//タックル。
					return e->GetState(TO_INT(Boss_Fatman::EnStateEX::enAttackD));
				}
				else {
					//距離をとる。
					return e->GetState(TO_INT(Boss_Fatman::EnStateEX::enTakeDistance));
				}
			}
		}
	}
	return this;
}

void Boss_FatmanMainState::Exit(IEnemy* e)
{
}

IEnemyState* Boss_FatmanMainState::LongRangeAttack(IEnemy* e)
{
	//確率で行動変化。
	auto rand = GRandom().Rand();
	if (rand < 0.33f) {
		//ビーム。
		return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
	}
	else if (rand < 0.66f) {
		//溜めビーム。
		return e->GetState(TO_INT(IEnemy::EnState::enAttackB));
	}
	else {
		//乱射。
		return e->GetState(TO_INT(Boss_Fatman::EnStateEX::enAttackC));
	}
}

void Boss_FatmanMainState::ChangeBattleState(IEnemy* e)
{
	const float percentage_Tired = 2.0f / 5.0f;
	const float percentage_Mad = 2.0f / 3.0f;
	if (e->GetCurrentHP() < e->GetMaxHP() * percentage_Tired) {
		Boss_Fatman::SetBattlePhase(EnBattlePhase::Tired);
	}
	else if (e->GetCurrentHP() < e->GetMaxHP() * percentage_Mad) {
		Boss_Fatman::SetBattlePhase(EnBattlePhase::Mad);
	}
}