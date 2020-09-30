#pragma once
#include "IEnemy.h"
class EnemyIdleState;

class EnemyTest : public IEnemy
{
public:
	virtual bool Start() override final;
	virtual void Update() override final;
	virtual void Release() override final;
	virtual void OnDestroy() override final;
private:
	EnemyIdleState* m_idleState = nullptr;
};

