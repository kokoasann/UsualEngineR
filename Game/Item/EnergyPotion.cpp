#include "stdafx.h"
#include "EnergyPotion.h"
#include "Player/Player.h"

EnergyPotion::EnergyPotion() {
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_upAxis = enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(m_scale);
	m_model->SetMulColor(m_color);

}

EnergyPotion::~EnergyPotion() {
	DeleteGO(m_model);
}

void EnergyPotion::Apply(Player* player) {
	player->ChargeBoost(player->GetMaxBoost());
}

void EnergyPotion::Update() {
	m_model->SetPosition(m_position);
}

