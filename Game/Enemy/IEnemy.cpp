#include "stdafx.h"
#include "IEnemy.h"
#include "State/IEnemyState.h"
#include "State/EnemyIdleState.h"
#include "State/EnemyBattleState.h"
#include "State/EnemySlashState.h"
#include "State/EnemyDeadState.h"
#include "HealthBar.h"

void IEnemy::Awake() {
	m_stateList.resize(static_cast<int>(EnState::enNumState));
	m_stateList[static_cast<int>(EnState::enIdleState)] = new EnemyIdleState();
	m_stateList[static_cast<int>(EnState::enBattleState)] = new EnemyBattleState();
	m_stateList[static_cast<int>(EnState::enAttackSlash)] = new EnemySlashState();
	m_stateList[static_cast<int>(EnState::enDeadState)] = new EnemyDeadState();
}

bool IEnemy::Start() {
	Init();
	if (m_isDrawHpBarAboveMyself) {
		m_healthBar = NewGO<HealthBar>(0);
	}
	return true;
}

void IEnemy::Release() {
	Terminate();
	std::for_each(m_stateList.begin(), m_stateList.end(), [&](IEnemyState* state) { delete state; state = nullptr; });
	if (m_healthBar != nullptr)
		DeleteGO(m_healthBar);
}

void IEnemy::OnDestroy() {
	Release();
}

void IEnemy::Update() {
	Execute();
	m_nextState = m_currentState->Update(this);

	//update hp bar
	if (m_healthBar != nullptr) {
		m_healthBar->SetParentPos(m_position);
		m_healthBar->SetHealthScale(GetCurrentHP() / GetMaxHP());
	}

	//体力がなくなったら死亡ステートへ遷移
	if (m_ability.hp <= 0) {
		m_nextState = m_stateList[static_cast<int>(EnState::enDeadState)];
	}

	if (m_nextState != m_currentState) {
		m_currentState->Exit(this);
		m_currentState = m_nextState;
		m_currentState->Enter(this);
	}
}

void IEnemy::SetState(IEnemyState* s) {

	if (m_currentState == nullptr) {
		m_currentState = s;
		m_currentState->Enter(this);
		return;
	}

	if (s == m_currentState) return;
	m_currentState->Exit(this);
	m_currentState = s;
	m_currentState->Enter(this);
}

void IEnemy::ApplyDamage(const float damage) {
	m_ability.hp = max(0.f, m_ability.hp - damage);
	if(m_healthBar != nullptr)
		m_healthBar->ShowHealthBar();
}