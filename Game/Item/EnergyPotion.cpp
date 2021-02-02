#include "stdafx.h"
#include "EnergyPotion.h"
#include "Player/Player.h"
#include "Effect/SmokeEffect.h"

EnergyPotion::EnergyPotion() {
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_upAxis = enUpAxisY;

	m_effect = NewGO<SmokeEffect>(0);
	//effect�̐F���P���������Ƃ���1.0f����̐��l�ɐݒ�B
	//��density��2.f�������ƕ\������Ȃ��悤�ł�
	m_effect->Init(Vector4(1.91f, 1.94f, 0.5f, 0.7f), Vector4(0.92f, 0.93f, 0.2f, 0.f), /*lifeSpanParam*/0.6f, /*density*/2.f, false);
	m_effect->SetSca(Vector3::One * 0.15f);
	//���Ƃ��ăv���C���[�̈ʒu
	//��ɂ܂��U�炷�B
	m_effect->SetPos(m_position);
	m_effect->Play();

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(m_scale);
	m_model->SetMulColor(m_color);

}

EnergyPotion::~EnergyPotion() {
	DeleteGO(m_model);
	if (m_effect != nullptr) {
		m_effect->Stop();
		DeleteGO(m_effect);
	}
}

void EnergyPotion::Apply(Player* player) {
	player->ChargeBoost(player->GetMaxBoost());
}

void EnergyPotion::Update() {
	m_model->SetPosition(m_position);
	m_effect->SetPos(m_position);
}

