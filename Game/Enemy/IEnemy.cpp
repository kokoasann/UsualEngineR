#include "stdafx.h"
#include "IEnemy.h"
#include "IEnemyState.h"
#include "EnemyIdleState.h"
#include "EnemyBattleState.h"

void IEnemy::Awake() {
	m_stateList.resize(EnState::enNumState);
	m_stateList[enIdleState] = new EnemyIdleState();
	m_stateList[enBattleState] = new EnemyBattleState();
}

bool IEnemy::Start() {
	Init();
	return true;
}

void IEnemy::Release() {
	Terminate();
	std::for_each(m_stateList.begin(), m_stateList.end(), [&](IEnemyState* state) { delete state; state = nullptr; });
}

void IEnemy::OnDestroy() {
	Release();
}

void IEnemy::Update() {
	m_nextState = m_currentState->Update(this);
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