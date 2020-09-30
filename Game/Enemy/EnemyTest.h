#pragma once
#include "IEnemy.h"
class EnemyIdleState;

class EnemyTest : public IEnemy
{
public:
	void Init() override;
	void Execute() override;
	void Terminate() override;
private:
	//model
	//position etc
};

