#include "stdafx.h"
#include "Boss_FatmanBeamState.h"
#include "Enemy/IEnemy.h"
#include "GameManager.h"
#include "Enemy/Boss/Boss_Fatman.h"

Boss_FatmanBeamState::Boss_FatmanBeamState()
{
}

Boss_FatmanBeamState::~Boss_FatmanBeamState()
{
}

//モデルに回転の反映はさせないかもしれない。
void Boss_FatmanBeamState::Enter(IEnemy* e)
{
	auto& p = GameManager::GetInstance().m_player;
	m_position = p->GetPosition();

	//少し傾ける。
	Quaternion rot;
	rot.SetRotationDeg(Vector3::AxisY, m_rotStartAngle);
	rot.Multiply(Boss_Fatman::EnemyToPlayerRotation(e));
	m_startRot = rot;
	e->GetModel()->SetRotation(rot);

	//回転角度のリセット。
	m_countRot = 0.f;

	//ダメージ数を計算。
	const float time = 20.f;
	m_damage = Boss_Fatman::CalcDamage(time);
}

IEnemyState* Boss_FatmanBeamState::Update(IEnemy* e)
{
	UpdateRotation(e);
	
	if (m_countRot > 0.f) {
		if (Judge(e)) {
			auto& p = GameManager::GetInstance().m_player;
			p->ApplyDamage(m_damage);
		}
	}

	return this;
}

void Boss_FatmanBeamState::Exit(IEnemy* e)
{
}

void Boss_FatmanBeamState::UpdateRotation(IEnemy* e)
{
	if (m_countRot < m_rotAngle) {
		//1秒間に行われるフレーム数。
		float frame = 1.f / gameTime()->GetDeltaTime();

		//1フレームあたりの回転量。
		float oneFrameAngle = m_rotAngle / m_rotTime / frame;
		
		//角度の合計量。
		m_countRot += oneFrameAngle;
		
		//回転。
		m_rotation.SetRotationDeg(Vector3::AxisY, m_countRot);
		Quaternion rot = m_rotation;
		rot.Multiply(m_startRot);
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
	m_rotation.Apply(vecEtoP);

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
	if (std::abs(dirW) < beamWidth and std::abs(dirH) < beamWidth and front > 0){
		return true;
	}
	return false;
}
