#include "stdafx.h"
#include "BossBombBattleState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "GameManager.h"
#include "Game.h"

#include "Camera/GameCamera.h"

BossBombBattleState::BossBombBattleState()
{
}

BossBombBattleState::~BossBombBattleState()
{
}

void BossBombBattleState::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(IEnemy::EnAnimation::enIdle));
	float perHP = e->GetCurrentHP()/e->GetMaxHP();
	if (perHP <= 2. / 3.)
	{
		//m_isAngry = true;
		m_timeLimit = 1.;
	}

	m_timer = 0.f;

	e->SetAutoRotateFlag(false);

	m_firstRot = e->GetRotation();

	if (!m_isPerformed) {
		GameManager::GetInstance().m_gameScene->OnEnterBattle(e);
		m_isPerformed = true;
	}
}

IEnemyState* BossBombBattleState::Update(IEnemy* e)
{
	if (!m_isAngry && BossBombData::GetInstance().feeling == BossBombData::EnFeel::Angry)
	{
		m_isAngry = true;
		m_timeLimit = 0.5;

		//e->PlayAnimation(TO_INT(Boss_MiddleBomb::EnAnimEX::));
		auto cam = GameManager::GetInstance().m_camera;
		auto tar = e->GetPosition();
		tar.y += 20.f;

		auto eneForward = e->GetForward();
		auto camEndPos = e->GetPosition() + eneForward * 35.f;
		camEndPos.y += 20.f;
		auto sec = 1.f;
		auto interval = 1.7f;

		g_graphicsEngine->GetPostEffect().SetUseFocusBlurFrag(true);

		cam->Perform(
			camEndPos, camEndPos,
			tar, tar, sec, interval, GameCamera::State::enEnemyCamera
		);
	}

	if (e->GetCurrentHP() <= 0.f)
		return e->GetState(TO_INT(IEnemy::EnState::enDeadState));
	float dtime = gameTime()->GetDeltaTime();
	m_timer += dtime;

	const auto& ppos = GameManager::GetInstance().GetPlayer()->GetPosition();
	const auto& epos = e->GetPosition();

	auto e2p = ppos - epos;
	float e2pLen = e2p.Length();

	float rand = GRandom().Rand();
	if (m_timer >= m_timeLimit && true)
	{
		if (e2pLen < m_distanceBash)
			return e->GetState(TO_INT(Boss_MiddleBomb::EnStateEX::Bash));

		float dify = ppos.y - epos.y;
		if (!GameManager::GetInstance().GetPlayer()->IsOnGround()&& dify > 50.f)
		{
			return e->GetState(TO_INT(Boss_MiddleBomb::EnStateEX::Jump));
		}
		
		if (rand < 0.4)
		{
			return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
		}
		else if (rand < 0.8)
		{
			return e->GetState(TO_INT(IEnemy::EnState::enAttackB));
		}
		else if (rand < 0.9 && false)
		{
			return e->GetState(TO_INT(Boss_MiddleBomb::EnStateEX::FullFrontal));
		}
	}

	Vector3 move = Vector3::Zero;

	
	auto e2pDir = e2p;
	e2pDir.y = 0.f;
	e2pDir.Normalize();

	if (e2pLen < m_distance-5.f)
	{
		move += e2pDir * (-m_speed * dtime * 0.5f);
	}
	else if (e2pLen > m_distance)
	{
		move += e2pDir * m_speed * dtime;
	}

	Quaternion rot;
	rot.SetRotationDegY(360.f * dtime);
	auto e2pRot = e2p;
	rot.Apply(e2pRot);
	e2pRot -= e2p;

	move += e2pRot;

	{
		float t = m_timer / m_timeRotate;
		if (t >= 1.f)
		{
			Quaternion rot;
			auto theta = atan2(e2p.x, e2p.z);
			rot.SetRotationY(theta);
			e->SetRotation(rot);
		}
		else
		{
			Quaternion rot;
			auto theta = atan2(e2p.x, e2p.z);
			//theta = theta * (180.f / Math::PI);
			rot.SetRotationY(theta);
			rot.Slerp(t, m_firstRot, rot);
			e->SetRotation(rot);
		}
	}

	e->SetVelocity(move);
	return this;
}

void BossBombBattleState::Exit(IEnemy* e)
{
	e->SetVelocity(Vector3::Zero);
}

void BossBombBattleState::OnAttacked(IEnemy* e)
{
}
