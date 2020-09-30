#include "stdafx.h"
#include "EnemyTest.h"
#include "IEnemyState.h"
#include "EnemyIdleState.h"

bool EnemyTest::Start() {
	m_idleState = new  EnemyIdleState;
	SetState(m_idleState);
	return true;
}

void EnemyTest::Release() {
	delete m_idleState;
}

void EnemyTest::OnDestroy() {
	Release();
}

void EnemyTest::Update() {
	//m_nextState = m_currentState->Update(this);
	//if (m_nextState != m_currentState) {
	//	m_currentState->Exit(this);
	//	m_currentState = m_nextState;
	//	m_currentState->Enter(this);
	//}
}