#include "stdafx.h"
#include "Boss_FatmanBeamState.h"
#include "Enemy/IEnemy.h"
#include "GameManager.h"
#include "Enemy/Boss/Boss_Fatman.h"
#include "Effect/Beam.h"

Boss_FatmanBeamState::Boss_FatmanBeamState()
{
	BeamEffectInitData bid;
	//骨の数だけビームを生成する。
	for (int i = 0; i < IKNum; i++) {
		Beam* beam = NewGO<Beam>(0);
		beam->Init(bid);
		beam->SetSca(Vector3::One * 0.03);
		m_beams.push_back(beam);
	}
}

Boss_FatmanBeamState::~Boss_FatmanBeamState()
{
	for (int i = 0; i < m_beams.size(), i++;) {
		DeleteGO(m_beams.at(i));
	}
}

//モデルに回転の反映はさせないかもしれない。
void Boss_FatmanBeamState::Enter(IEnemy* e)
{
	auto& p = GameManager::GetInstance().m_player;
	m_position = p->GetPosition();
	m_position.y += 6.0f;

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

	m_isState = false;

	//IK情報。
	for (int i = 0; i < IKNum; i++) {
		m_ik[i] = e->GetIK(TO_INT(IEnemy::EnIK::enArm_L) + i);
	}

	m_maxSenkei = 0;
}

IEnemyState* Boss_FatmanBeamState::Update(IEnemy* e)
{
	UpdateRotation(e);
	
	if (m_isState && m_maxSenkei > 1.0f) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	if (m_countRot > 0.f) {
		Judge(e);
	}

	//プレイヤーの横方向を求める。
	const auto epos = e->GetPosition();
	Vector3 vec = m_position - epos;
	vec.Normalize();
	Vector3 XZ;
	XZ.Cross(vec, Vector3::Up);

	//線形補完してプレイヤーの左から右へ動かす。
	Vector3 senkeihokan;
	const float moveRange = 1.0f;
	float oneFrameMove = moveRange / m_rotTime * gameTime()->GetDeltaTime();
	m_maxSenkei += oneFrameMove;
	if (m_maxSenkei <= 1.0f) {
		const float range = 37.0f;
		senkeihokan.Lerp(m_maxSenkei, XZ * range + m_position, XZ * -range + m_position);
	}

	for (int i = 0; i < IKNum; i++) {
		m_ik[i]->SetNextTarget(senkeihokan);
		m_beams[i]->Play();
		m_beams[i]->SetPos(m_ik[i]->GetEffectorBone()->GetWorldMatrix().GetTransrate());
		m_beams[i]->SetRot(m_ik[i]->GetEffectorBone()->GetWorldMatrix().GetRotate());
	}

	return this;
}

void Boss_FatmanBeamState::Exit(IEnemy* e)
{
}

void Boss_FatmanBeamState::EffectInit()
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
	else {
		m_isState = true;
	}
}

//ビームの幅の判定。
void Boss_FatmanBeamState::Judge(IEnemy* e)
{
	for (int i = 0; i < IKNum; i++) {
		//ビームを発射している方向を計算。
		//ボーンの前方向に回転量を割り当て、向きを計算。
		Vector3 vecBeamDir = Vector3::AxisY;
		m_ik[i]->GetEffectorBone()->GetWorldMatrix().GetRotate().Apply(vecBeamDir);

		//外積。横方向に伸びた、vecBeamDirに直行するベクトル。
		Vector3 EWidth;
		EWidth.Cross(vecBeamDir, Vector3::Up);
		EWidth.Normalize();

		//pposは現在のプレイヤーの位置。
		const auto& p = GameManager::GetInstance().m_player;
		const auto& ppos = p->GetPosition();
		Vector3 bonePos = m_ik[i]->GetEffectorBone()->GetWorldMatrix().GetTransrate();
		Vector3 vecArmtoCurrentP = ppos - bonePos;

		//プレイヤーと敵を並べたと仮定したときの距離。
		float distance = EWidth.Dot(vecArmtoCurrentP);

		//正面にいるかどうか判定するための内積。
		//マイナスだったら後ろ。
		float front = vecBeamDir.Dot(vecArmtoCurrentP);

		const float beamWidth = 15.0f;		//ビームの幅。
		if (std::abs(distance) < beamWidth and front > 0) {
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
}
