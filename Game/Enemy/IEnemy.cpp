#include "stdafx.h"
#include "IEnemy.h"
#include "State/IEnemyState.h"
#include "State/EnemyIdleState.h"
#include "State/EnemyBattleState.h"
#include "State/EnemySlashState.h"
#include "State/EnemyDeadState.h"

void IEnemy::Awake() {
	m_stateList.resize(static_cast<int>(EnState::enNumState));
	m_stateList[static_cast<int>(EnState::enIdleState)] = new EnemyIdleState();
	m_stateList[static_cast<int>(EnState::enBattleState)] = new EnemyBattleState();
	m_stateList[static_cast<int>(EnState::enAttackSlash)] = new EnemySlashState();
	m_stateList[static_cast<int>(EnState::enDeadState)] = new EnemyDeadState();
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
	Execute();
	m_nextState = m_currentState->Update(this);

	if (m_HP <= 0) {
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