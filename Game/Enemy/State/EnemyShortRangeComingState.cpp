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
	e->PlayAnimation(TO_INT(IEnemy::EnAnimation::enRun));

	if (m_headIK == nullptr)
	{
		m_headIK = e->GetIK(TO_INT(IEnemy::EnIK::enHead));
	}
}

IEnemyState* EnemyShortRangeComingState::Update(IEnemy* e)
{
	if(e->GetPreviousState() == e->GetState(TO_INT(Zako_ShortRangeMonster::EnStateEX::enLongJampAttack)))
		return e->GetState(TO_INT(Zako_ShortRangeMonster::EnStateEX::enDance));

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

	//�v���C���[���󒆂ɂ���Ȃ瓮���Ȃ��B
	const float airPlayerYPos = 70.0f;
	if (vecToPlayer.y > airPlayerYPos) {
		return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
	}

	const float moveRange = 10.f;
	const float attackRange = 30.f;
	const float idleRange = 200.f;

	const float vec2PlayerLen = vecToPlayer.Length();
	vecToPlayer /= vec2PlayerLen;

	if (player->GetCurrentHP() > 0.f) {

		float t = acosf(vecToPlayer.Dot(e->GetForward()));

		//�߂Â�����s���B
		if (vec2PlayerLen < attackRange && t < Math::PI * 0.3f) {
			auto rand = GRandom().Rand();
			//��30%�̊m���ōs���ω��B
			if (rand < 0.1f) {
				//�_���X�B
				return e->GetState(TO_INT(Zako_ShortRangeMonster::EnStateEX::enDance));
			}
			else if (rand < 0.7f) {
				//�p���`�B
				return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
			}
			else {
				//��U��̃p���`�B
				return e->GetState(TO_INT(IEnemy::EnState::enAttackB));
			}
		}
		else if (vec2PlayerLen < 35 && t < Math::PI * 0.3f)
		{
			return e->GetState(TO_INT(Zako_ShortRangeMonster::EnStateEX::enLongJampAttack));
		}
		//���ꂽ��ړ��B
		else {
			Move(e);
		}

		//���ꂽ��ړ���~�B
		if (vec2PlayerLen > idleRange) {
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
			m_danceTimer = 0.f;
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
	auto player = GameManager::GetInstance().GetPlayer();

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	vecToPlayer.y = 0.f;
	vecToPlayer.Normalize();
	
	const float walkSpeed = 60.0f;
	Vector3 moveSpeed = vecToPlayer * walkSpeed;

	//�d�́B
	if (!e->IsOnGround())
		moveSpeed.y -= 100.0f;
	else
		DebugPrint_NOMOTO("is on ground");
	
	e->SetVelocity(moveSpeed);
}

bool EnemyShortRangeComingState::Animation()
{
	return true;
}
