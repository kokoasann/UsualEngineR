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
}

IEnemyState* Boss_FatmanBeamState::Update(IEnemy* e)
{
	if (Judge(e)) {
		//m_damageTimer += gameTime()->GetDeltaTime();
		auto& p = GameManager::GetInstance().m_player;
		p->ApplyDamage(m_damage);
	}
	return this;
}

void Boss_FatmanBeamState::Exit(IEnemy* e)
{
}

bool Boss_FatmanBeamState::Judge(IEnemy* e)
{
	//�r�[���̕��̔���B
	auto& epos = e->GetPosition();

	//�G����v���C���[�Ɍ������x�N�g���B
	//m_position�͍ŏ��Ƀ��b�N�I�������Ƃ��̃v���C���[�̈ʒu�B
	Vector3 vecEtoP = m_position - epos;

	//�O�ρB�������ɐL�т�VecPtoE�ɒ��s����x�N�g���B
	Vector3 ECross;
	ECross.Cross(vecEtoP, Vector3::Up);
	ECross.Normalize();

	//ppos�͌��݂̃v���C���[�̈ʒu�B
	auto& p = GameManager::GetInstance().m_player;
	auto& ppos = p->GetPosition();
	Vector3 vecEtoCurrentP = ppos - epos;

	//�v���C���[�ƓG�����ɕ��ׂ��Ɖ��肵���Ƃ��̋����B
	float dir = ECross.Dot(vecEtoCurrentP);

	//���ʂɂ��邩�ǂ������肷�邽�߂̓��ρB
	//�}�C�i�X����������B
	float front = vecEtoP.Dot(vecEtoCurrentP);

	const float beamWidth = 20.0f;		//�r�[���̕��B
	if (std::abs(dir) < beamWidth and front > 0){
		return true;
	}
	return false;
}
