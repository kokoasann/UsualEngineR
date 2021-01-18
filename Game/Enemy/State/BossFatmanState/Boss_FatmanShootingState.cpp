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
}

IEnemyState* Boss_FatmanShootingState::Update(IEnemy* e)
{
	const float shotTime = 0.01f;
	m_shotTimer += gameTime()->GetDeltaTime();
	m_stateTimer += gameTime()->GetDeltaTime();
	const float maxTime = 5.f;
	if (m_stateTimer > maxTime) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	//ˆê’èŠÔŠu‚Å’e‚ðŒ‚‚ÂB
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
	//Œ‚‚¿‚¾‚·•ûŒü‚ðŒˆ‚ß‚éB
	//­‚µã‚ÉÝ’èB
	const float upY = 6.f;
	Enemy_Bullet* eb = NewGO<Enemy_Bullet>(0, true);
	auto epos = e->GetPosition();
	epos.y += upY;

	auto& p = GameManager::GetInstance().m_player;
	auto ppos = p->GetPosition();
	ppos.y += 4.f;

	Vector3 dir = ppos - epos;
	dir.Normalize();

	//Œ‚‚¿‚¾‚·’e‚ÌU‚ê•‚ðŒˆ‚ß‚éB(1‚Â–Ú‚Ì‰ñ“])
	Vector3 cross;
	cross.Cross(dir, Vector3::Up);

	//ƒ‰ƒ“ƒ_ƒ€‚ÈŠp“xB
	const float maxAngle = 4.f;
	float angle = 0.f;
	auto rand = GRandom().Rand();
	angle = maxAngle * rand;

	//1‚Â–Ú‚Ì‰ñ“]‚ðì¬B
	Quaternion rot;
	rot.SetRotationDeg(cross, angle);

	//ŽåŽ²‚ð‰ñ“]‚³‚¹‚éB(2‚Â–Ú‚Ì‰ñ“])
	//ƒ‰ƒ“ƒ_ƒ€‚ÈŠp“xB
	const float maxAngle2 = 360.f;
	float angle2 = 0.f;
	auto rand2 = GRandom().Rand();
	angle2 = maxAngle2 * rand2;

	//2‚Â–Ú‚Ì‰ñ“]‚ðì¬B
	Quaternion rot2;
	rot2.SetRotationDeg(dir, angle2);

	//‰ñ“]‚ð“K—pB
	rot.Apply(dir);
	rot2.Apply(dir);

	//’e‚Ì‰Šú‰»B
	eb->Init(epos, 0.6f, dir, 400.f, 5.f, 2.f);
	//ƒvƒŒƒCƒ„[‚ª”ò‚ñ‚Å‚¢‚½‚çŒ‚‚¿—Ž‚Æ‚·B
	Vector3 knockVector = ppos - epos;
	knockVector.Normalize();
	const float knockParam = 200.f;
	knockVector *= knockParam;
	if (!p->IsOnGround()) {
		eb->SetDamage(m_damage, true, knockVector);
	}
	else {
		eb->SetDamage(m_damage);
	}
}
