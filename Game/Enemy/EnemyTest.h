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
	//Model
	ModelRender* m_model = nullptr;
	CAnimationClip m_animClip[1];
	std::vector<CAnimationClipPtr> m_animlist;
};

