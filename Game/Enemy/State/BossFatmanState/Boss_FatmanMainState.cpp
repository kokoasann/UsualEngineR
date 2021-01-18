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
	//�d�́B
	Vector3 gravity = Vector3::Zero;
	gravity.y = -5000.f;
	e->SetVelocity(gravity);
	
	m_fatTimer = 0.f;

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);
}
IEnemyState* Boss_FatmanMainState::Update(IEnemy* e)
{
	m_fatTimer += gameTime()->GetDeltaTime();
	if (m_fatTimer > 0.3f) {
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
			//���ꂽ�牓�����U���B
			//�������̓v���C���[�����ł����牓�����U���B
			if (vecToPlayer.Length() > attackRange or diffY > flyRange) {
				return LongRangeAttack(e);
			}
			//�߂Â�����^�b�N�����������Ƃ�B
			else {
				//�m���ōs���ω��B
				auto rand = GRandom().Rand();
				if (rand < 0.5f) {
					//�^�b�N���B
					return e->GetState(TO_INT(Boss_Fatman::EnStateEX::enAttackD));
				}
				else {
					//�������Ƃ�B
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
	//�m���ōs���ω��B
	auto rand = GRandom().Rand();
	if (rand < 0.33f) {
		//�r�[���B
		return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
	}
	else if (rand < 0.66f) {
		//���߃r�[���B
		return e->GetState(TO_INT(IEnemy::EnState::enAttackB));
	}
	else {
		//���ˁB
		return e->GetState(TO_INT(Boss_Fatman::EnStateEX::enAttackC));
	}
}
