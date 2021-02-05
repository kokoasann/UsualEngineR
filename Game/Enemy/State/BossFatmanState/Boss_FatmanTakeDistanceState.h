#pragma once
#include "Enemy/State/IEnemyState.h"

class Boss_FatmanTakeDistanceState final : public IEnemyState
{
public:
	Boss_FatmanTakeDistanceState();
	~Boss_FatmanTakeDistanceState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	SphereCollider		m_sphereCollider;
	const float			m_radius = 3.0f;
	CSoundSource* m_takeDistSE = nullptr;

};
