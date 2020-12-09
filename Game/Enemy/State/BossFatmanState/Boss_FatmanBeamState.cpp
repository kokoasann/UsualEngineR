#include "stdafx.h"
#include "Boss_FatmanBeamState.h"
#include "Enemy/IEnemy.h"
#include "GameManager.h"

Boss_FatmanBeamState::Boss_FatmanBeamState()
{
}

Boss_FatmanBeamState::~Boss_FatmanBeamState()
{
}

void Boss_FatmanBeamState::Enter(IEnemy* e)
{
	auto& p = GameManager::GetInstance().m_player;
	m_position = p->GetPosition();

	PreRotation(e);

	//回転角度のリセット。
	m_countRot = 0.f;
}

void Boss_FatmanBeamState::PreRotation(IEnemy* e)
{
	//横回転と縦回転を計算し、プレイヤーの方向を向くようにしている。
	auto& p = GameManager::GetInstance().m_player;
	const auto& ppos = p->GetPosition();
	const auto& epos = e->GetPosition();
	Vector3 vecToPlayer = ppos - epos;

	//横回転。
	float angleW = atan2(vecToPlayer.x, vecToPlayer.z);
	Quaternion rot;
	rot.SetRotation(Vector3::AxisY, angleW);

	//縦回転。
	Vector3 vecToPlayerXZ = vecToPlayer;
	vecToPlayerXZ.y = 0.0f;
	vecToPlayer.Normalize();
	vecToPlayerXZ.Normalize();
	float dot = vecToPlayer.Dot(vecToPlayerXZ);
	float angleH = acos(dot);

	//回転軸。
	Vector3 axis;
	axis.Cross(vecToPlayer, vecToPlayerXZ);
	axis.Normalize();

	Quaternion rot2;
	rot2.SetRotation(axis, -angleH);

	//回転の合成。
	rot.Multiply(rot2);

	//少し傾ける。
	Quaternion rot3;
	rot3.SetRotationDeg(Vector3::AxisY, -90.f);
	rot.Multiply(rot3);

	m_rotation = rot;
	
	e->GetModel()->SetRotation(rot);
}


IEnemyState* Boss_FatmanBeamState::Update(IEnemy* e)
{
	if (Judge(e)) {
		//m_damageTimer += gameTime()->GetDeltaTime();
		auto& p = GameManager::GetInstance().m_player;
		p->ApplyDamage(m_damage);
	}

	UpdateRotation(e);

	return this;
}

void Boss_FatmanBeamState::Exit(IEnemy* e)
{
}


void Boss_FatmanBeamState::UpdateRotation(IEnemy* e)
{
	const float maxTime = 5.0f;
	const float maxAngle = 180.0f;

	if (m_countRot < maxAngle) {
		//1秒間に行われるフレーム数。
		float frame = 1.f / gameTime()->GetDeltaTime();

		//1フレームあたりの回転量。
		float oneFrameAngle = maxAngle / maxTime / frame;
		
		//未回転の角度量。
		m_countRot += oneFrameAngle;
		
		//回転。
		Quaternion rot;
		rot.SetRotationDeg(Vector3::AxisY, m_countRot);
		rot.Multiply(m_rotation);
		e->GetModel()->SetRotation(rot);		
	}
}

bool Boss_FatmanBeamState::Judge(IEnemy* e)
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

	const float beamWidth = 20.0f;		//ビームの幅。
	if (std::abs(dirW) < beamWidth and std::abs(dirH) < beamWidth and front > 0){
		return true;
	}
	return false;
}
