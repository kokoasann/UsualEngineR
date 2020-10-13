#include "stdafx.h"
#include "EnemyIdleState.h"
#include "../IEnemy.h"
#include <cmath>

EnemyIdleState::EnemyIdleState() {

}

EnemyIdleState::~EnemyIdleState() {

}

void EnemyIdleState::Enter(IEnemy* e) {
	printf("Enter enemy Idle\n");
}

IEnemyState* EnemyIdleState::Update(IEnemy* e) {

	//�K���ɓ������Ă݂�����
	static float time = 0.f;
	time += gameTime()->GetDeltaTime();
	auto add = std::sin(time);
	auto nextPos = e->GetPosition();
	nextPos.x += add * 0.05;
	e->SetPosition(nextPos);

	//�ʂ̏�ԃN���X�����^�[�����ď�Ԃ�J�ڂ�����
	/*
	if (g_pad[0]->IsTrigger(enButtonA)) {
		return e->GetState(IEnemy::EnState::enBattleState);
	}
	*/

	//��Ԃ��ς��Ȃ��Ȃ玩����Ԃ�
	return this;
}

void EnemyIdleState::Exit(IEnemy* e) {
	printf("Exit enemy Idle\n");
}