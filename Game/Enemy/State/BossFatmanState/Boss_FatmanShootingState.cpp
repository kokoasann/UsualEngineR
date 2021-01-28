#include "stdafx.h"
#include "Boss_FatmanShootingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "GameManager.h"
#include "Enemy/Equipment/Enemy_Bullet.h"
#include "Enemy/Boss/Boss_Fatman.h"

Boss_FatmanShootingState::Boss_FatmanShootingState()
{
}

Boss_FatmanShootingState::~Boss_FatmanShootingState()
{
}

void Boss_FatmanShootingState::Enter(IEnemy* e)
{
	const float time = 20.f;
	m_damage = Boss_Fatman::CalcDamage(time);

	m_shotTimer = 0.f;
	m_stateTimer = 0.f;

	//IKèÓïÒÅB
	for (int i = 0; i < IKNum; i++) {
		m_ik[i] = e->GetIK(TO_INT(IEnemy::EnIK::enFoot_L) + i);
	}
}

IEnemyState* Boss_FatmanShootingState::Update(IEnemy* e)
{
	e->GetModel()->SetRotation(Boss_Fatman::EnemyToPlayerRotation(e));

	const float shotTime = 0.01f;
	m_shotTimer += gameTime()->GetDeltaTime();
	m_stateTimer += gameTime()->GetDeltaTime();
	const float maxTime = 5.f;
	if (m_stateTimer > maxTime) {
		m_isKnockBack = false;
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	//àÍíËä‘äuÇ≈íeÇåÇÇ¬ÅB
	if (m_shotTimer > shotTime) {
		auto rand = GRandom().Rand();
		rand *= 3.f;
		for (int i = 0; i <= rand; i++){
			BulletGenerate(e);
		}
		
		m_shotTimer = 0.f;
	}
	return this;
}

void Boss_FatmanShootingState::Exit(IEnemy* e)
{
}

void Boss_FatmanShootingState::BulletGenerate(IEnemy* e)
{
	for (int i = 0; i < IKNum; i++) {
		//åÇÇøÇæÇ∑ï˚å¸ÇåàÇﬂÇÈÅB
		//è≠Çµè„Ç…ê›íËÅB
		const float upY = 6.f;
		Enemy_Bullet* eb = NewGO<Enemy_Bullet>(0, true);
		/*auto epos = e->GetPosition();
		epos.y += upY;*/
		Vector3 epos = m_ik[i]->GetEffectorBone()->GetWorldMatrix().GetTransrate();

		auto& p = GameManager::GetInstance().m_player;
		auto ppos = p->GetPosition();
		ppos.y += 6.0f;

		for (int i = 0; i < IKNum; i++) {
			m_ik[i]->SetNextTarget(ppos);
		}

		Vector3 dir = ppos - epos;
		dir.Normalize();

		//åÇÇøÇæÇ∑íeÇÃêUÇÍïùÇåàÇﬂÇÈÅB(1Ç¬ñ⁄ÇÃâÒì])
		Vector3 cross;
		cross.Cross(dir, Vector3::Up);

		//ÉâÉìÉ_ÉÄÇ»äpìxÅB
		const float maxAngle = 4.f;
		float angle = 0.f;
		auto rand = GRandom().Rand();
		angle = maxAngle * rand;

		//1Ç¬ñ⁄ÇÃâÒì]ÇçÏê¨ÅB
		Quaternion rot;
		rot.SetRotationDeg(cross, angle);

		//éÂé≤ÇâÒì]Ç≥ÇπÇÈÅB(2Ç¬ñ⁄ÇÃâÒì])
		//ÉâÉìÉ_ÉÄÇ»äpìxÅB
		const float maxAngle2 = 360.f;
		float angle2 = 0.f;
		auto rand2 = GRandom().Rand();
		angle2 = maxAngle2 * rand2;

		//2Ç¬ñ⁄ÇÃâÒì]ÇçÏê¨ÅB
		Quaternion rot2;
		rot2.SetRotationDeg(dir, angle2);

		//âÒì]ÇìKópÅB
		rot.Apply(dir);
		rot2.Apply(dir);

		//íeÇÃèâä˙âªÅB
		eb->Init(epos, 0.6f, dir, 400.f, 5.f, 2.f);

		//çUåÇÅB
		//ÉvÉåÉCÉÑÅ[Ç™îÚÇÒÇ≈Ç¢ÇΩÇÁåÇÇøóéÇ∆Ç∑ÅB
		Vector3 knockVector = ppos - epos;
		knockVector.Normalize();
		const float knockParam = 200.f;
		knockVector *= knockParam;
		if (!m_isKnockBack && !p->IsOnGround()) {
			eb->SetDamage(m_damage, true, knockVector);
			m_isKnockBack = false;
		}
		else {
			eb->SetDamage(m_damage);
		}
	}
}
