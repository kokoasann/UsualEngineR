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
	m_effect->Init(Vector4(1.91f, 1.94f, 0.5f, 0.05f), Vector4(0.92f, 0.93f, 0.2f, 0.f), /*lifeSpanParam*/0.6f, /*density*/2.f, false);
	m_effect->SetSca(Vector3::One * 0.15f);
	//���Ƃ��ăv���C���[�̈ʒu
	//��ɂ܂��U�炷�B
	m_effect->SetPos(m_position);
	m_effect->SetSpeed(10.0f);
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
	CSoundSource* se = NewGO<CSoundSource>(0, "Punch");
	se->Init(L"Assets/sound/Heal.wav");
	se->Play(false);
	se->SetVolume(1.0f);
}

void EnergyPotion::Update() {
	//deltaTime���߂�ϐ�
	m_deltaTimeSt += gameTime()->GetDeltaTime();
	//�T�C���J�[�u�̕ω��ʂ������ 
	float sinValue = sin(m_PI * 2 * m_deltaTimeSt);
	//sinValue *= gameTime()->GetDeltaTime();
	const float rotateSpeed = 120.0f;
	m_angle += rotateSpeed * gameTime()->GetDeltaTime();


	Vector3 updownPos = m_position;
	updownPos.y += sinValue;
	m_model->SetPosition(updownPos);
	m_effect->SetPos(updownPos);

	Quaternion m_qrot;
	m_qrot.SetRotationDeg(Vector3::AxisY, m_angle);
	m_model->SetRotation(m_qrot);
}

