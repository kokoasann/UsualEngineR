#include "stdafx.h"
#include "Boss_FatmanTakeDistanceState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_Fatman.h"

Boss_FatmanTakeDistanceState::Boss_FatmanTakeDistanceState()
{
	m_sphereCollider.Create(m_radius);
	m_takeDistSE = NewGO<CSoundSource>(0);
	m_takeDistSE->Init(L"Assets/sound/chara/footstep1.wav");
}

Boss_FatmanTakeDistanceState::~Boss_FatmanTakeDistanceState()
{
}

void Boss_FatmanTakeDistanceState::Enter(IEnemy* e)
{
	const auto& rot = Boss_Fatman::EnemyToPlayerRotation(e, false);
	e->GetModel()->SetRotation(rot);
	e->PlayAnimation(TO_INT(Boss_Fatman::EnAnimEX::enbackStep));
}

IEnemyState* Boss_FatmanTakeDistanceState::Update(IEnemy* e)
{
	//���f���̉�]�B
	e->GetModel()->SetRotation(Boss_Fatman::EnemyToPlayerRotation(e));

	//�v�Z�̂��߂̃f�[�^�B
	auto& p = GameManager::GetInstance().m_player;
	const auto& epos = e->GetPosition();
	const auto& ppos = p->GetPosition();
	auto vecToBoss = epos - ppos;
	float distance = vecToBoss.Length();
	
	//�������Ƃ�Ă��邩����B
	if (std::abs(distance) > Boss_Fatman::TAKE_DISTANCE){
		e->SetVelocity(Vector3::Zero);
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	//�{�X�Ɉ�ԋ߂��Z���𒲍��B
	auto allCell = GameManager::GetInstance().m_nvm.GetCell();
	Vector3 enemyDiff = allCell[0]->centerPos - epos;
	Cell* bossCell = allCell[0];	//�{�X�Ɉ�ԋ߂��Z���B
	for (auto& all : allCell)
	{
		//newEnemyDiff�̍X�V
		Vector3 newEnemyDiff = all->centerPos - epos;

		//enemy�����ԋ߂��Z�������߂�
		//enemyDiff��苗�����Z���Z������������
		if (enemyDiff.Length() > newEnemyDiff.Length())
		{
			//���ƃZ����o�^
			enemyDiff = newEnemyDiff;
			bossCell = all;
		}
	}

	//�{�X�̂���Z���̗אڃZ������v���C���[�܂�
	//��Ԓ��������̗אڃZ���𒲂ׂ�B
	const int vertexNum = 3;		//�Z���̒��_�̐��B
	Cell* longDistLinkCell = nullptr;	//��Ԓ��������̗אڃZ���B
	int startCellNo;
	//�����ݒ�B
	for (int i = 0; i < vertexNum; i++) {
		if (bossCell->linkCells[i] != NULL) {
			longDistLinkCell = bossCell->linkCells[i];
			startCellNo = i;
			break;
		}
	}
	//�����J�n�B
	for (int i = startCellNo + 1; i < vertexNum; i++) {
		auto vec1 = longDistLinkCell->centerPos - ppos;
		float dist1 = vec1.Length();

		Vector3 vec2;
		if (bossCell->linkCells[i] != NULL) {
			vec2 = bossCell->linkCells[i]->centerPos - ppos;
		}
		float dist2 = vec2.Length();

		if (dist1 < dist2) {
			longDistLinkCell = bossCell->linkCells[i];
		}
	}

	//�ړ��B
	auto vecBossToCell = longDistLinkCell->centerPos - epos;
	const float	ARRIVAL_DISTANCE =	5.0f;		//�����������ǂ������肷�邽�߂̋����B
	if (vecBossToCell.Length() > ARRIVAL_DISTANCE) {
		vecBossToCell.Normalize();
		const float applyMovespeed = 100.0f;
		Vector3 movespeed = vecBossToCell * applyMovespeed;
		Vector3 diff = longDistLinkCell->centerPos - epos;
		e->SetVelocity(movespeed);
	}
	else {
		e->SetVelocity(Vector3::Zero);
	}
	return this;
}

void Boss_FatmanTakeDistanceState::Exit(IEnemy* e)
{
}
