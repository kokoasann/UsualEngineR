#include "stdafx.h"
#include "EnemyLongBigAttackState.h"
#include "EnemyLongBigTargetingState.h"
#include "Enemy/Zako/Zako_LongBig.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Equipment/Enemy_Bullet.h"
#include "Effect/MuzzleFlash.h"
#include "Effect/Beam.h"

#define BULLET 0;
#define BEAM 1;


EnemyLongBigAttackState::~EnemyLongBigAttackState()
{
#if BULLET
	DeleteGO(m_muzzleFlash);
#elif BEAM
	DeleteGO(m_beam);
#endif
}

void EnemyLongBigAttackState::Init(IK* ik, float bulletSpeed, EnemyLongBigTargetingState* lbt)
{
#if BULLET
	m_ik = ik;
	m_speed = bulletSpeed;
	m_lbt = lbt;
	m_muzzleFlash = NewGO<MuzzleFlash>(0);
	MuzzleFlashEffectInitData mfid;
	m_muzzleFlash->Init(mfid);
	m_muzzleFlash->SetSca(Vector3::One * 0.1);
#elif BEAM
	m_lbt = lbt;
	m_beam = NewGO<Beam>(0);
	BeamEffectInitData bid;
	m_beam->Init(bid);
	m_beam->SetSca(Vector3::One * 0.03);
#endif
}

void EnemyLongBigAttackState::Enter(IEnemy* e)
{
	m_timer = 0.f;
	m_shotCount = 0;

	m_velocity = m_target - e->GetPosition();
	m_velocity.Normalize();

	e->PlayAnimation(TO_INT(Zako_LongBig::EnAnimEX::Fire));
}

IEnemyState* EnemyLongBigAttackState::Update(IEnemy* e)
{
#if BULLET
	float dtime = gameTime()->GetDeltaTime();
	m_timer += dtime;
	m_ik->SetNextTarget(m_target);

	if (m_timer > 2.f)
		return m_lbt;
	//���ԂɂȂ�����e�𔭎ˁB
	if (m_timer >= m_timeSpan && m_shotCount < m_maxShot)
	{
		Enemy_Bullet* eb = NewGO<Enemy_Bullet>(0, true);
		auto bone = m_ik->GetEffectorBone();
		const auto& mat = bone->GetWorldMatrix();
		auto p = mat.GetTransrate();
		auto rot = mat.GetRotate();

		m_muzzleFlash->Play();
		m_muzzleFlash->SetPos(p);
		m_muzzleFlash->SetRot(rot);


		//p.y += 10.;
		eb->Init(p, 0.25f, m_velocity, m_speed, 5, 1);
		eb->SetDamage(Player::GetMaxHP() / 60.f / 9.f);
		m_timer = 0.f;
		m_shotCount++;
		if (m_shotCount >= m_maxShot)
		{
			//return m_ldt;
		}
	}
	return this;
#elif BEAM
	if (Charge(e)) {
		m_endBeamTimer += gameTime()->GetDeltaTime();
		const float endTime = 1.f;		//�����Ă��鎞�ԁB
		if (m_endBeamTimer < endTime) {
			//�r�[������������ݒ�B
			if (!m_isSetPos) {
				auto& p = GameManager::GetInstance().m_player;
				m_position = p->GetPosition();
				m_isSetPos = true;
			}
			//�U������B
			if (BeamJudge(e)) {
				//�v���C���[�����ł����猂�����Ƃ��B
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
			return m_lbt;
		}
	}
	return this;
#endif
}

void EnemyLongBigAttackState::Exit(IEnemy* e)
{
}

bool EnemyLongBigAttackState::Charge(IEnemy* e)
{
	const float chargeTime = 1.f;		//���߂Ă��鎞�ԁB
	m_chargeTimer += gameTime()->GetDeltaTime();
	if (m_chargeTimer > chargeTime) {
		return true;
	}

	//�G�t�F�N�g�̐ݒ�B
	//�G�t�F�N�g�𗭂ߏ�Ԃɐݒ�B
	m_beam->SetChange(true);

	//�傫���������߂ɐݒ�B
	m_beam->SetSca(Vector3::One * 0.03);

	m_beam->SetPos(e->GetPosition());

	//���f������v���C���[�܂ł̕�����ݒ�B
	auto& p = GameManager::GetInstance().m_player;
	const auto& ppos = p->GetPosition();
	Vector3 vecEtoP = ppos - e->GetPosition();
	m_beam->SetToPlayerDir(vecEtoP);

	//�O�ρB���s�����c�̃x�N�g���B
	Vector3 EHeight;
	EHeight.Cross(vecEtoP, Vector3::Right);
	EHeight.Normalize();
	m_beam->SetHolizontalDir(EHeight);

	//�G�t�F�N�g�̍Đ��B
	m_beam->Play();

	//���f�����v���C���[�̕����������悤�ɐݒ�B
	//����]�B
	float angleW = atan2(vecEtoP.x, vecEtoP.z);
	Quaternion rot;
	rot.SetRotation(Vector3::AxisY, angleW);
	e->GetModel()->SetRotation(rot);
	return false;
}

bool EnemyLongBigAttackState::BeamJudge(IEnemy* e)
{
	//�G�t�F�N�g�̗��ߏ�Ԃ������B
	m_beam->SetChange(false);
	//�傫����傫�߂ɐݒ�B
	m_beam->SetSca(Vector3::One * 0.3);

	//�r�[���̕��̔���B
	//�G����v���C���[�Ɍ������x�N�g���B
	//m_position�͍ŏ��Ƀ��b�N�I�������Ƃ��̃v���C���[�̈ʒu�B

	Vector3 vecEtoP = m_position - e->GetPosition();
	m_beam->SetToPlayerDir(vecEtoP);

	//�O�ρB�������ɐL�т��AVecPtoE�ɒ��s����x�N�g���B
	Vector3 EWidth;
	EWidth.Cross(vecEtoP, Vector3::Up);
	EWidth.Normalize();

	//�O�ρB�c�����ɐL�т��AVecPtoE�ɒ��s����x�N�g���B
	Vector3 EHeight;
	EHeight.Cross(vecEtoP, Vector3::Right);
	EHeight.Normalize();

	//ppos�͌��݂̃v���C���[�̈ʒu�B
	auto& p = GameManager::GetInstance().m_player;
	auto& ppos = p->GetPosition();
	Vector3 vecEtoCurrentP = ppos - e->GetPosition();

	//�v���C���[�ƓG�����ɕ��ׂ��Ɖ��肵���Ƃ��̋����B
	float dirW = EWidth.Dot(vecEtoCurrentP);

	//�v���C���[�ƓG���c�ɕ��ׂ��Ɖ��肵���Ƃ��̋����B
	float dirH = EHeight.Dot(vecEtoCurrentP);

	//���ʂɂ��邩�ǂ������肷�邽�߂̓��ρB
	//�}�C�i�X����������B
	float front = vecEtoP.Dot(vecEtoCurrentP);

	m_beam->Play();
	m_beam->SetPos(e->GetPosition());

	const float beamWidth = 15.0f;		//�r�[���̕��B
	if (std::abs(dirW) < beamWidth and std::abs(dirH) < beamWidth and front > 0) {
		return true;
	}
	return false;
}