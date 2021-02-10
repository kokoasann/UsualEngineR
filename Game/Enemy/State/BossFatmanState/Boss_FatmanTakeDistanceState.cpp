#include "stdafx.h"
#include "Boss_FatmanTakeDistanceState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_Fatman.h"

Boss_FatmanTakeDistanceState::Boss_FatmanTakeDistanceState()
{
	m_sphereCollider.Create(m_radius);
}

Boss_FatmanTakeDistanceState::~Boss_FatmanTakeDistanceState()
{
}

void Boss_FatmanTakeDistanceState::Enter(IEnemy* e)
{
	const auto& rot = Boss_Fatman::EnemyToPlayerRotation(e, false);
	e->GetModel()->SetRotation(rot);
	
	e->PlayAnimation(TO_INT(Boss_Fatman::EnAnimEX::enbackStep));
	
	CSoundSource* se = NewGO<CSoundSource>(0);
	se->Init(L"Assets/sound/boss_fatman/backstep.wav");
	se->Play(false);
}

IEnemyState* Boss_FatmanTakeDistanceState::Update(IEnemy* e)
{
	//モデルの回転。
	e->GetModel()->SetRotation(Boss_Fatman::EnemyToPlayerRotation(e));

	//計算のためのデータ。
	auto& p = GameManager::GetInstance().m_player;
	const auto& epos = e->GetPosition();
	const auto& ppos = p->GetPosition();
	auto vecToBoss = epos - ppos;
	float distance = vecToBoss.Length();
	
	//距離をとれているか判定。
	if (std::abs(distance) > Boss_Fatman::TAKE_DISTANCE){
		e->SetVelocity(Vector3::Zero);
		return e->GetState(TO_INT(IEnemy::EnState::enStunState));
	}

	Cell* longDistLinkCell = nullptr;	//一番長い距離の隣接セル。
	if (!m_isArrival) {
		//ボスに一番近いセルを調査。
		auto allCell = GameManager::GetInstance().m_nvm.GetCell();
		Vector3 enemyDiff = allCell[0]->centerPos - epos;
		Cell* bossCell = allCell[0];	//ボスに一番近いセル。
		for (auto& all : allCell)
		{
			//newEnemyDiffの更新
			Vector3 newEnemyDiff = all->centerPos - epos;

			//enemyから一番近いセルを求める
			//enemyDiffより距離が短いセルがあったら
			if (enemyDiff.Length() > newEnemyDiff.Length())
			{
				//差とセルを登録
				enemyDiff = newEnemyDiff;
				bossCell = all;
			}
		}

		//ボスのいるセルの隣接セルからプレイヤーまで
		//一番長い距離の隣接セルを調べる。
		const int vertexNum = 3;		//セルの頂点の数。
		int startCellNo;
		//初期設定。
		for (int i = 0; i < vertexNum; i++) {
			if (bossCell->linkCells[i] != NULL) {
				longDistLinkCell = bossCell->linkCells[i];
				startCellNo = i;
				break;
			}
		}
		//調査開始。
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
		m_isArrival = true;
		m_longDistLinkCell = longDistLinkCell;
	}

	//移動。
	Vector3 vecBossToCell = m_longDistLinkCell->centerPos - epos;
	const float	ARRIVAL_DISTANCE =	5.0f;		//到着したかどうか判定するための距離。
	float length = vecBossToCell.Length();
	if (length > ARRIVAL_DISTANCE) {
		vecBossToCell.Normalize();
		const float applyMovespeed = 80.0f;
		Vector3 movespeed = vecBossToCell * applyMovespeed;
		Vector3 diff = m_longDistLinkCell->centerPos - epos;
		e->SetVelocity(movespeed);
	}
	else {
		m_isArrival = false;
		m_count++;
		e->SetVelocity(Vector3::Zero);
		if (m_count >= 3) {
			return e->GetState(TO_INT(IEnemy::EnState::enStunState));
		}
	}
	
	return this;
}

void Boss_FatmanTakeDistanceState::Exit(IEnemy* e)
{
}
