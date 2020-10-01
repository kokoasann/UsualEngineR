#include "stdafx.h"
#include "EnemyIdleState.h"
#include "IEnemy.h"
#include <cmath>

EnemyIdleState::EnemyIdleState() {

}

EnemyIdleState::~EnemyIdleState() {

}

void EnemyIdleState::Enter(IEnemy* e) {
	printf("Enter enemy Idle\n");
}

IEnemyState* EnemyIdleState::Update(IEnemy* e) {

	//適当に動かしてみただけ
	static float time = 0.f;
	time += gameTime()->GetDeltaTime();
	auto add = std::sin(time);
	auto nextPos = e->GetPosition();
	nextPos.x += add * 0.05;
	e->SetPosition(nextPos);

	//別の状態クラスをリターンして状態を遷移させる
	/*
	if (g_pad[0]->IsTrigger(enButtonA)) {
		return e->GetState(IEnemy::EnState::enBattleState);
	}
	*/

	//状態が変わらないなら自分を返す
	return this;
}

void EnemyIdleState::Exit(IEnemy* e) {
	printf("Exit enemy Idle\n");
}