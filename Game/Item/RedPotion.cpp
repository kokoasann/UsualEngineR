#include "stdafx.h"
#include "RedPotion.h"
#include "../Player/Player.h"
#include "Effect/SmokeEffect.h"

RedPotion::RedPotion() {
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_upAxis = enUpAxisY;

	m_effect = NewGO<SmokeEffect>(0);
	//��density��2.f�������ƕ\������Ȃ��悤�ł�
	m_effect->Init(Vector4(1.9f, 0.4f, 0.4f, 0.8f), Vector4(0.9f, 0.2f, 0.2f, 0.f), /*lifeSpanParam*/0.6f, /*density*/2.f, false);
	m_effect->SetSca(Vector3::One * 0.15f);
	//���Ƃ��ăv���C���[�̈ʒu
	//��ɂ܂��U�炷�B
	m_effect->Play();

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(m_scale);
	m_model->SetMulColor(m_color);
}

RedPotion::~RedPotion() {
	DeleteGO(m_model);
	if (m_effect != nullptr) {
		m_effect->Stop();
		DeleteGO(m_effect);
	}
}

void RedPotion::Apply(Player* player) {

	player->Heal(m_healAmount);
	DebugPrint_WATA("Heal\n");
	CSoundSource* se = NewGO<CSoundSource>(0, "Punch");
	se->Init(L"Assets/sound/Heal.wav");
	se->Play(false);
	se->SetVolume(1.0f);
}

void RedPotion::Update() {
	//�T�C���J�[�u�̃J�E���g�p�ϐ�
	m_count++;

	//�T�C���J�[�u�̕ω��ʂ������ 60�̂Ƃ����deltaTime()��������Ȃ������̂ŉ��̐��l
	float sinValue = sin(m_PI * 2 / gameTime()->GetDeltaTime() * m_count);
	const float rotateSpeed = 300.0f;
	m_angle += rotateSpeed * gameTime()->GetDeltaTime();

	Vector3 updownPos = m_position;
	updownPos.y += sinValue;
	m_model->SetPosition(updownPos);
	m_effect->SetPos(updownPos);

	Quaternion m_qrot;
	m_qrot.SetRotationDeg(Vector3::AxisY, m_angle);
	m_model->SetRotation(m_qrot);

}

