#include "stdafx.h"
#include "Boss_FatmanChargeBeamState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_Fatman.h"

Boss_FatmanChargeBeamState::Boss_FatmanChargeBeamState()
{
}

Boss_FatmanChargeBeamState::~Boss_FatmanChargeBeamState()
{
}

void Boss_FatmanChargeBeamState::Enter(IEnemy* e)
{
	auto& p = GameManager::GetInstance().m_player;
	m_position = p->GetPosition();

	e->GetModel()->SetRotation(Boss_Fatman::EnemyToPlayerRotation(e));

	//ダメージ数を計算。
	const float time = 10.f;
	m_damage = Boss_Fatman::CalcDamage(time);

	//タイマーのリセット。
	m_chargeTimer = 0.f;
	m_endBeamTimer = 0.f;
}

IEnemyState* Boss_FatmanChargeBeamState::Update(IEnemy* e)
{
	if (Charge()) {
		m_endBeamTimer += gameTime()->GetDeltaTime();
		const float endTime = 2.f;
		if (m_endBeamTimer < endTime) {
			if (Beam(e)) {
				//プレイヤーが飛んでいたら撃ち落とす。
				const float flyRange = 5.f;
				auto epos = e->GetPosition();
				auto& p = GameManager::GetInstance().m_player;
				auto ppos = p->GetPosition();
				if (std::abs(ppos.y - epos.y) > flyRange) {
					p->ApplyDamage(m_damage, true, Vector3::Zero);
				}
				else {
					p->ApplyDamage(m_damage);
				}
			}
		}
		else {
			return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
		}
	}
	return this;
}

void Boss_FatmanChargeBeamState::Exit(IEnemy* e)
{
}

bool Boss_FatmanChargeBeamState::Charge()
{
	const float chargeTime = 3.f;
	m_chargeTimer += gameTime()->GetDeltaTime();
	if (m_chargeTimer > chargeTime) {
		return true;
	}
	return false;
}

bool Boss_FatmanChargeBeamState::Beam(IEnemy* e)
{
	//ビームの幅の判定。
	auto& epos = e->GetPosition();

	//敵からプレイヤーに向かうベクトル。
	//m_positionは最初にロックオンしたときのプレイヤーの位置。
	Vector3 vecEtoP = m_position - epos;

	//外積。横方向に伸びた、VecPtoEに直行するベクトル。
	Vector3 EWidth;
	EWidth.Cross(vecEtoP, Vector3::Up);
	EWidth.Normalize();

	//外積。縦方向に伸びた、VecPtoEに直行するベクトル。
	Vector3 EHeight;
	EHeight.Cross(vecEtoP, Vector3::Right);
	EHeight.Normalize();

	//pposは現在のプレイヤーの位置。
	auto& p = GameManager::GetInstance().m_player;
	auto& ppos = p->GetPosition();
	Vector3 vecEtoCurrentP = ppos - epos;

	//プレイヤーと敵を横に並べたと仮定したときの距離。
	float dirW = EWidth.Dot(vecEtoCurrentP);

	//プレイヤーと敵を縦に並べたと仮定したときの距離。
	float dirH = EHeight.Dot(vecEtoCurrentP);

	//正面にいるかどうか判定するための内積。
	//マイナスだったら後ろ。
	float front = vecEtoP.Dot(vecEtoCurrentP);

	const float beamWidth = 15.0f;		//ビームの幅。
	if (std::abs(dirW) < beamWidth and std::abs(dirH) < beamWidth and front > 0) {
		return true;
	}
	return false;
}
