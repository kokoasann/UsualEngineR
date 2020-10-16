#include "stdafx.h"
#include "HealthBar.h"


HealthBar::HealthBar()
{

}

HealthBar::~HealthBar()
{

}



void HealthBar::Release()
{
	DeleteGO(m_spHp);
}

void HealthBar::OnDestroy()
{
	Release();
}


void HealthBar::Awake()
{

}

bool HealthBar::Start()
{
	SpriteInitData sd;
	m_spHp = NewGO<SpriteRender>(0);
	sd.m_ddsFilePath[0] = "Assets/Image/hp.dds";
	sd.m_height = m_flSpHpHeight;
	sd.m_width = m_flSpHpWidth;
	m_spHp->Init(sd);
	m_spHp->SetSca(m_hpScale);
	m_spHp->SetPivot(m_SPRITE_PIVOT);
	m_spHp->MainCameraView();
	m_spHp->Mode_BillBord();
	return true;
}


void HealthBar::PreUpdate()
{

}

void HealthBar::Update()
{

	if (!m_shouldBeDrawn) return;

	auto eyePos = g_camera3D->GetPosition();
	auto vecEyeToParent = m_parentPos - eyePos;
	vecEyeToParent.y = 0.f;
	vecEyeToParent.Normalize();
	vecEyeToParent.Cross(Vector3::Up);
	auto right = vecEyeToParent;
	auto hpPos = m_parentPos + m_hpPosOffset + right * m_flSpHpWidth / 2.f;
	m_spHp->SetPos(hpPos);
	m_spHp->SetSca(m_hpScale);

	m_drawTimer += gameTime()->GetDeltaTime();

	printf("timer : %f\n", m_drawTimer);

	if (m_drawTimer >= m_drawSec) {
		m_shouldBeDrawn = false;
		m_spHp->SetActive(false);
		printf("bye\n");
	}

}

void HealthBar::PostUpdate()
{

}


void HealthBar::Render()
{

}

void HealthBar::PostRender()
{

}
