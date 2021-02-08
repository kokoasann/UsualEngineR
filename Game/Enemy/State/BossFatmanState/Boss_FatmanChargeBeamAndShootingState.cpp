#include "stdafx.h"
#include "Boss_FatmanChargeBeamAndShootingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_Fatman.h"
#include "Effect/Beam.h"
#include "Enemy/Equipment/Enemy_Bullet.h"

Boss_FatmanChargeBeamAndShootingState::Boss_FatmanChargeBeamAndShootingState()
{
	//骨の数だけビームを生成する。
	for (int i = 0; i < IK_NUM; i++) {
		Beam* beam = NewGO<Beam>(0);
		BeamEffectInitData bid;
		beam->Init(bid);
		beam->SetSca(Vector3::One * 0.03);
		m_beams.push_back(beam);
	}
	m_chargeSE = NewGO<CSoundSource>(0, "sound");
	m_chargeSE->Init(L"Assets/sound/boss_fatman/charge.wav");
	m_shootSE = NewGO<CSoundSource>(0, "sound");
	m_shootSE->Init(L"Assets/sound/boss_fatman/Balkan.wav");
}

Boss_FatmanChargeBeamAndShootingState::~Boss_FatmanChargeBeamAndShootingState()
{
	//生成した数だけ削除。
	for (int i = 0; i < m_beams.size(); i++) {
		DeleteGO(m_beams.at(i));
	}
	DeleteGO(m_chargeSE);
	DeleteGO(m_shootSE);
}

void Boss_FatmanChargeBeamAndShootingState::Enter(IEnemy* e)
{
	//e->GetModel()->SetRotation(Boss_Fatman::EnemyToPlayerRotation(e));
	m_startRot = e->GetModel()->GetRotation();
	m_sumLerp = 0.0f;
	
	InitChargeBeam(e);

	InitShooting(e);
}

IEnemyState* Boss_FatmanChargeBeamAndShootingState::Update(IEnemy* e)
{
	//球面線形補完を使ってじわじわプレイヤーに向かって回転させる。
	Quaternion EtoPRot = Boss_Fatman::EnemyToPlayerRotation(e);
	Quaternion currentRot = Quaternion::Identity;
	const float rotTime = 1.0f;
	m_sumLerp += gameTime()->GetDeltaTime();
	currentRot.Slerp(min(1.0f, m_sumLerp / rotTime), m_startRot, EtoPRot);
	e->GetModel()->SetRotation(currentRot);

	ChargeBeam(e);

	Shooting(e);

	if (m_isEndChargeBeam && m_isEndShooting) {
		m_isKnockBackCB = false;
		m_isKnockBackSH = false;
		m_isBeamSound = false;
		m_shootSE->Stop();
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}
	
	return this;
}

void Boss_FatmanChargeBeamAndShootingState::Exit(IEnemy* e)
{
}

void Boss_FatmanChargeBeamAndShootingState::OnAttacked(IEnemy* e)
{
}

void Boss_FatmanChargeBeamAndShootingState::InitChargeBeam(IEnemy* e)
{
	//ダメージ数を計算。
	const float time = 10.f;
	m_chargeDamage = Boss_Fatman::CalcDamage(time);

	//タイマーのリセット。
	m_chargeTimer = 0.f;
	m_endBeamTimer = 0.f;

	m_isSetPos = false;

	//IK情報。
	for (int i = 0; i < IK_NUM; i++) {
		m_chargebeamIk[i] = e->GetIK(TO_INT(IEnemy::EnIK::enArm_L) + i);
	}
	m_isEndChargeBeam = false;
}

void Boss_FatmanChargeBeamAndShootingState::ChargeBeam(IEnemy* e)
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
				m_position.y += 6.f;
			}
			//ビームは2本あるので2回判定を行う。
			for (int i = 0; i < IK_NUM; i++) {
				//攻撃判定。
				if (BeamJudge(e, i)) {
					//プレイヤーが飛んでいたら撃ち落とす。
					auto& p = GameManager::GetInstance().m_player;
					if (!m_isKnockBackCB && !p->IsOnGround()) {
						p->ApplyDamage(m_chargeDamage, true, Vector3::Zero);
						m_isKnockBackCB = true;
					}
					else {
						p->ApplyDamage(m_chargeDamage);
					}
					m_endBeamTimer = 0.f;
				}
			}
		}
		else {
			m_isEndChargeBeam = true;
		}
	}

}

bool Boss_FatmanChargeBeamAndShootingState::Charge(IEnemy* e)
{
	float chargeTime = 0.0f;		//溜めている時間。
	if (EnBattlePhase::Mad == Boss_Fatman::GetCurrentBattlePhase()) {
		chargeTime = 1.0f;
	}
	else if (EnBattlePhase::Tired == Boss_Fatman::GetCurrentBattlePhase()) {
		chargeTime = 2.0f;
	}


	m_chargeTimer += gameTime()->GetDeltaTime();
	if (m_chargeTimer > chargeTime) {
		m_isChargeSound = false;
		m_chargeSE->Stop();
		return true;
	}

	if (!m_isChargeSound) {
		m_chargeSE->Play(false);
		m_chargeSE->SetVolume(0.08f);
		m_isChargeSound = true;
	}

	for (int i = 0; i < IK_NUM; i++) {
		//エフェクトの設定。
		//エフェクトを溜め状態に設定。
		m_beams[i]->SetChange(true);

		//大きさを小さめに設定。
		m_beams[i]->SetSca(Vector3::One * 0.03);

		//骨の座標の取得。
		auto epos = m_chargebeamIk[i]->GetEffectorBone()->GetWorldMatrix().GetTransrate();
		m_beams[i]->SetPos(epos);

		//モデルからプレイヤーまでの方向を設定。
		auto& p = GameManager::GetInstance().m_player;
		auto ppos = p->GetPosition();

		//狙いを少し上にする。
		const float yUp = 4.f;
		ppos.y += yUp;

		//エフェクトの再生。
		m_beams[i]->Play();
		m_beams[i]->SetRot(m_chargebeamIk[i]->GetEffectorBone()->GetWorldMatrix().GetRotate());
		//IKで砲身を動かす。
		m_chargebeamIk[i]->SetNextTarget(ppos);
	}
	return false;
}

bool Boss_FatmanChargeBeamAndShootingState::BeamJudge(IEnemy* e, int ikNo)
{
	//エフェクトの溜め状態を解除。
	m_beams[ikNo]->SetChange(false);
	//大きさを大きめに設定。
	m_beams[ikNo]->SetSca(Vector3::One * 0.3);

	//ビームの幅の判定。
	//敵からプレイヤーに向かうベクトル。
	//m_positionは最初にロックオンしたときのプレイヤーの位置。
	auto epos = m_chargebeamIk[ikNo]->GetEffectorBone()->GetWorldMatrix().GetTransrate();

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
	auto ppos = p->GetPosition();
	ppos.y += 6.0f;
	Vector3 vecEtoCurrentP = ppos - epos;

	//プレイヤーと敵を横に並べたと仮定したときの距離。
	float dirW = EWidth.Dot(vecEtoCurrentP);

	//プレイヤーと敵を縦に並べたと仮定したときの距離。
	float dirH = EHeight.Dot(vecEtoCurrentP);

	//正面にいるかどうか判定するための内積。
	//マイナスだったら後ろ。
	float front = vecEtoP.Dot(vecEtoCurrentP);

	//IKで砲身を動かす。
	/*const float yUp = 4.f;
	m_position.y += yUp;*/

	Vector3 movespeed = ppos - m_position;
	movespeed.Normalize();
	const float addmovespeed = 10.0f;
	movespeed *= addmovespeed * gameTime()->GetDeltaTime();
	m_position += movespeed;
	m_chargebeamIk[ikNo]->SetNextTarget(m_position);

	m_beams[ikNo]->SetPos(epos);
	m_beams[ikNo]->SetRot(m_chargebeamIk[ikNo]->GetEffectorBone()->GetWorldMatrix().GetRotate());
	m_beams[ikNo]->Play();

	if (!m_isBeamSound) {
		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/beam.wav");
		se->Play(false);
		m_isBeamSound = true;
	}

	const float beamWidth = 15.0f;		//ビームの幅。
	if (std::abs(dirW) < beamWidth and std::abs(dirH) < beamWidth and front > 0) {
		return true;
	}
	return false;
}

void Boss_FatmanChargeBeamAndShootingState::InitShooting(IEnemy* e)
{
	const float time = 20.f;
	m_shootingDamage = Boss_Fatman::CalcDamage(time);

	m_shotTimer = 0.f;
	m_stateTimer = 0.f;

	//IK情報。
	for (int i = 0; i < IK_NUM; i++) {
		m_shootingIk[i] = e->GetIK(TO_INT(IEnemy::EnIK::enFoot_L) + i);
	}

	m_isEndShooting = false;
}

void Boss_FatmanChargeBeamAndShootingState::Shooting(IEnemy* e)
{
	const float shotTime = 0.01f;
	m_shotTimer += gameTime()->GetDeltaTime();
	m_stateTimer += gameTime()->GetDeltaTime();
	const float maxTime = 5.f;
	if (m_stateTimer > maxTime) {
		m_isEndShooting = true;
	}

	//一定間隔で弾を撃つ。
	if (m_shotTimer > shotTime) {
		auto rand = GRandom().Rand();
		rand *= 3.f;
		for (int i = 0; i <= rand; i++) {
			BulletGenerate(e);
			if (!m_shootSE->IsPlaying()) {
				m_shootSE->Play(true);
			}
		}

		m_shotTimer = 0.f;
	}
}

void Boss_FatmanChargeBeamAndShootingState::BulletGenerate(IEnemy* e)
{
	for (int i = 0; i < IK_NUM; i++) {
		//撃ちだす方向を決める。
		//少し上に設定。
		const float upY = 6.f;
		Enemy_Bullet* eb = NewGO<Enemy_Bullet>(0, true);
		/*auto epos = e->GetPosition();
		epos.y += upY;*/
		Vector3 epos = m_shootingIk[i]->GetEffectorBone()->GetWorldMatrix().GetTransrate();

		auto& p = GameManager::GetInstance().m_player;
		auto ppos = p->GetPosition();
		ppos.y += 6.0f;
		
		for (int i = 0; i < IK_NUM; i++) {
			m_shootingIk[i]->SetNextTarget(ppos);
		}

		Vector3 dir = ppos - epos;
		dir.Normalize();

		//撃ちだす弾の振れ幅を決める。(1つ目の回転)
		Vector3 cross;
		cross.Cross(dir, Vector3::Up);

		//ランダムな角度。
		const float maxAngle = 4.f;
		float angle = 0.f;
		auto rand = GRandom().Rand();
		angle = maxAngle * rand;

		//1つ目の回転を作成。
		Quaternion rot;
		rot.SetRotationDeg(cross, angle);

		//主軸を回転させる。(2つ目の回転)
		//ランダムな角度。
		const float maxAngle2 = 360.f;
		float angle2 = 0.f;
		auto rand2 = GRandom().Rand();
		angle2 = maxAngle2 * rand2;

		//2つ目の回転を作成。
		Quaternion rot2;
		rot2.SetRotationDeg(dir, angle2);

		//回転を適用。
		rot.Apply(dir);
		rot2.Apply(dir);

		//弾の初期化。
		eb->Init(epos, 0.6f, dir, 400.f, 5.f, 2.f);

		//攻撃。
		//プレイヤーが飛んでいたら撃ち落とす。
		Vector3 knockVector = ppos - epos;
		knockVector.Normalize();
		const float knockParam = 200.f;
		knockVector *= knockParam;
		if (!m_isKnockBackSH && !p->IsOnGround()) {
			eb->SetDamage(m_shootingDamage, true, knockVector);
			m_isKnockBackSH = true;
		}
		else {
			eb->SetDamage(m_shootingDamage);
		}
	}
}