#include "stdafx.h"
#include "EnemyShortRangeComingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Zako/Zako_ShortRangeMonster.h"

EnemyShortRangeComingState::EnemyShortRangeComingState()
{
}

EnemyShortRangeComingState::~EnemyShortRangeComingState()
{
}

void EnemyShortRangeComingState::Enter(IEnemy* e)
{
	e->PlayAnimation(IEnemy::EnAnimation::enWalk);
}

IEnemyState* EnemyShortRangeComingState::Update(IEnemy* e)
{
	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;

	//�v���C���[���󒆂ɂ���Ȃ瓮���Ȃ��B
	const float airPlayerYPos = 70.0f;
	if (ppos.y > airPlayerYPos) {
		return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
	}

	const float moveRange = 10.f;
	const float attackRange = 5.f;
	const float idleRange = 100.f;

	if (player->GetCurrentHP() > 0.f) {
		//�߂Â�����s���B
		if (vecToPlayer.Length() < attackRange) {
			auto rand = GRandom().Rand();
			//��30%�̊m���ōs���ω��B
			if (rand < 0.1f) {
				//�_���X�B
				return e->GetState(TO_INT(Zako_ShortRangeMonster::EnStateEX::enDance));
			}
			else if (rand < 0.55f) {
				//�p���`�B
				return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
			}
			else {
				//��U��̃p���`�B
				return e->GetState(TO_INT(IEnemy::EnState::enAttackB));
			}
		}
		//���ꂽ��ړ��B
		else {
			Move(e);
		}

		//���ꂽ��ړ���~�B
		if (vecToPlayer.Length() > idleRange) {
			return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
		}
	}

	//�����Ă���r���ł��܂ɗx��B
	const float maxTime = 2.5f;
	m_danceTimer += gameTime()->GetDeltaTime();
	if (m_danceTimer > maxTime) {
		auto rand = GRandom().Rand();
		//��15%�̊m���ŗx��B
		if (rand < 0.15f) {
			//�_���X�B
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
	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	vecToPlayer.Normalize();
	
	const float walkSpeed = 40.0f;
	Vector3 moveSpeed = vecToPlayer * walkSpeed;

	//�d�́B
	moveSpeed.y -= 100.0f;
	
	e->SetVelocity(moveSpeed);
}

bool EnemyShortRangeComingState::Animation()
{
	return true;
}
