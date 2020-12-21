#include "stdafx.h"
#include "Boss_FatmanChargeBeamState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_Fatman.h"
#include "Effect/Beam.h"

Boss_FatmanChargeBeamState::Boss_FatmanChargeBeamState()
{
	//骨の数だけビームを生成する。
	for (int i = 0; i < BoneNum; i++) {
		Beam* beam = NewGO<Beam>(0);
		BeamEffectInitData bid;
		beam->Init(bid);
		beam->SetSca(Vector3::One * 0.03);
		m_beams.push_back(beam);
	}
}

Boss_FatmanChargeBeamState::~Boss_FatmanChargeBeamState()
{
	//生成した数だけ削除。
	for (int i = 0; i < m_beams.size(), i++;) {
		DeleteGO(m_beams.at(i));
	}
}

void Boss_FatmanChargeBeamState::Enter(IEnemy* e)
{
	e->GetModel()->SetRotation(Boss_Fatman::EnemyToPlayerRotation(e));

	//ダメージ数を計算。
	const float time = 10.f;
	m_damage = Boss_Fatman::CalcDamage(time);

	//タイマーのリセット。
	m_chargeTimer = 0.f;
	m_endBeamTimer = 0.f;

	m_isSetPos = false;

	//骨情報。
	auto ske = e->GetModel()->GetModel().GetSkelton();
	m_bone[Right] = ske->GetBone(ske->FindBoneID(L"Beam_IK_R"));
	m_bone[Left] = ske->GetBone(ske->FindBoneID(L"Beam_IK_L"));
}

IEnemyState* Boss_FatmanChargeBeamState::Update(IEnemy* e)
{
	if (Charge(e)) {
		m_endBeamTimer += gameTime()->GetDeltaTime();
		const float endTime = 2.f;		//撃っている時間。
		if (m_endBeamTimer < endTime) {
			//ビームを撃つ方向を設定。
			if (!m_isSetPos) {
				auto& p = GameManager::GetInstance().m_player;
				m_position = p->GetPosition();
				m_isSetPos = true;
			}
			//ビームは2本あるので2回判定を行う。
			for (int i = 0; i < BoneNum; i++) {
				//攻撃判定。
				if (BeamJudge(e,i)) {
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
		else {
			return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
		}
	}
	return this;
}

void Boss_FatmanChargeBeamState::Exit(IEnemy* e)
{
}

bool Boss_FatmanChargeBeamState::Charge(IEnemy* e)
{
	const float chargeTime = 5.f;		//溜めている時間。
	m_chargeTimer += gameTime()->GetDeltaTime();
	if (m_chargeTimer > chargeTime) {
		return true;
	}

	for (int i = 0; i < BoneNum; i++) {
		//エフェクトの設定。
		//エフェクトを溜め状態に設定。
		m_beams[i]->SetChange(true);

		//大きさを小さめに設定。
		m_beams[i]->SetSca(Vector3::One * 0.03);

		//骨の座標の取得。
		const auto epos = m_bone[i]->GetWorldMatrix().GetTransrate();
		m_beams[i]->SetPos(epos);

		//モデルからプレイヤーまでの方向を設定。
		auto& p = GameManager::GetInstance().m_player;
		const auto& ppos = p->GetPosition();
		Vector3 vecEtoP = ppos - epos;
		m_beams[i]->SetToPlayerDir(vecEtoP);

		//外積。直行した縦のベクトル。
		Vector3 EHeight;
		EHeight.Cross(vecEtoP, Vector3::Right);
		EHeight.Normalize();
		m_beams[i]->SetHolizontalDir(EHeight);

		//エフェクトの再生。
		m_beams[i]->Play();
	}
	
	//モデルがプレイヤーの方向を向くように設定。
	e->GetModel()->SetRotation(Boss_Fatman::EnemyToPlayerRotation(e));	
	return false;
}

bool Boss_FatmanChargeBeamState::BeamJudge(IEnemy* e, int boneNo)
{
	//エフェクトの溜め状態を解除。
	m_beams[boneNo]->SetChange(false);
	//大きさを大きめに設定。
	m_beams[boneNo]->SetSca(Vector3::One * 0.3);

	//ビームの幅の判定。
	//敵からプレイヤーに向かうベクトル。
	//m_positionは最初にロックオンしたときのプレイヤーの位置。
	auto epos = m_bone[boneNo]->GetWorldMatrix().GetTransrate();

	Vector3 vecEtoP = m_position - epos;
	m_beams[boneNo]->SetToPlayerDir(vecEtoP);

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

	m_beams[boneNo]->Play();
	m_beams[boneNo]->SetPos(epos);

	const float beamWidth = 15.0f;		//ビームの幅。
	if (std::abs(dirW) < beamWidth and std::abs(dirH) < beamWidth and front > 0) {
		return true;
	}
	return false;
}
