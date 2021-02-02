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
	//effectの色を輝かせたいときは1.0fより上の数値に設定。
	//※densityは2.f未満だと表示されないようです
	m_effect->Init(Vector4(1.91f, 1.94f, 0.5f, 0.05f), Vector4(0.92f, 0.93f, 0.2f, 0.f), /*lifeSpanParam*/0.6f, /*density*/2.f, false);
	m_effect->SetSca(Vector3::One * 0.15f);
	//仮としてプレイヤーの位置
	//上にまき散らす。
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
	//deltaTimeためる変数
	m_deltaTimeSt += gameTime()->GetDeltaTime();
	//サインカーブの変化量をいれる 
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

