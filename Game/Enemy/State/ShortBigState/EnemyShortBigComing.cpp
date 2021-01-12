#include "stdafx.h"
#include "EnemyShortBigComing.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Zako/Zako_ShortBig.h"
#include "GameManager.h"

EnemyShortBigComing::EnemyShortBigComing()
{
}

EnemyShortBigComing::~EnemyShortBigComing()
{
}

void EnemyShortBigComing::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(IEnemy::EnAnimation::enRun));

	if (m_headIK == nullptr)
	{
		m_headIK = e->GetIK(TO_INT(IEnemy::EnIK::enHead));
	}
}

//ジャンプアタック関連以外はEnemyShortRangeComingStateと同じ。」
IEnemyState* EnemyShortBigComing::Update(IEnemy* e)
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
	const float attackRange = 10.f;
	const float idleRange = 150.f;

	const float vec2PlayerLen = vecToPlayer.Length();
	vecToPlayer /= vec2PlayerLen;

	if (player->GetCurrentHP() > 0.f) {

		float t = acosf(vecToPlayer.Dot(e->GetForward()));

		//近づいたら行動。
		if (vec2PlayerLen < attackRange && t < Math::PI * 0.3f) {
			auto rand = GRandom().Rand();
			//約30%の確率で行動変化。
			if (rand < 0.05f) {
				//ダンス。
				return e->GetState(TO_INT(Zako_ShortBig::EnStateEX::enDance));
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
		//離れすぎたら移動停止。
		if (vec2PlayerLen > idleRange) {
			return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
		}
	}

	//歩いている途中でたまに踊る。
	const float maxTime = 2.5f;
	m_danceTimer += gameTime()->GetDeltaTime();
	if (m_danceTimer > maxTime) {
		auto rand = GRandom().Rand();
		//約5%の確率で踊る。
		if (rand < 0.05f) {
			m_danceTimer = 0.f;
			//ダンス。
			return e->GetState(TO_INT(Zako_ShortBig::EnStateEX::enDance));
		}
		m_danceTimer = 0.f;
	}

	return this;
}

void EnemyShortBigComing::Exit(IEnemy* e)
{
	e->SetVelocity(Vector3::Zero);
}

void EnemyShortBigComing::Move(IEnemy* e)
{
	auto player = GameManager::GetInstance().GetPlayer();

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	vecToPlayer.y = 0.f;
	vecToPlayer.Normalize();

	//普通のゴブリンの速度の1/2　マジックナンバーは変更予定
	const float walkSpeed = 55.0f;
	Vector3 moveSpeed = vecToPlayer * walkSpeed;

	//重力。
	if (!e->IsOnGround())
		moveSpeed.y -= 100.0f;
	else
		DebugPrint_NOMOTO("is on ground");

	e->SetVelocity(moveSpeed);
}

bool EnemyShortBigComing::IsAnimation()
{
	return true;
}