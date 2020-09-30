#include "stdafx.h"
#include "IEnemy.h"
#include "IEnemyState.h"

bool IEnemy::Start() {
	return true;
}

void IEnemy::Release() {

}

void IEnemy::OnDestroy() {

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