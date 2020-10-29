#include "stdafx.h"
#include "RedPotion.h"
#include "../Player/Player.h"

RedPotion::RedPotion() {
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_upAxis = enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(m_scale);
}

RedPotion::~RedPotion() {
	DeleteGO(m_model);
}

void RedPotion::Apply(Player* player) {
	player->Heal(m_healAmount);
	printf("Heal\n");
}

void RedPotion::Update() {
	m_model->SetPosition(m_position);
}

