#include "stdafx.h"
#include "EnemyTest.h"
#include "IEnemyState.h"

void EnemyTest::Init() {
	/*
	AllocConsole();

	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
	*/

	SetState(m_stateList[enIdleState]);
}


void EnemyTest::Terminate() {
}

void EnemyTest::Execute() {

}

