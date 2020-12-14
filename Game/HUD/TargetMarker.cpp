#include "stdafx.h"
#include "TargetMarker.h"


TargetMarker::TargetMarker()
{

}

TargetMarker::~TargetMarker()
{

}



void TargetMarker::Release()
{
	DeleteGO(m_spMarker);
}

void TargetMarker::OnDestroy()
{

}


void TargetMarker::Awake()
{

}

bool TargetMarker::Start()
{

	SpriteInitData sd;
	m_spMarker = NewGO<SpriteRender>(0);
	sd.m_ddsFilePath[0] = "Assets/Image/targetMarker.dds";
	sd.m_height = m_flSpHpHeight;
	sd.m_width = m_flSpHpWidth;
	m_spMarker->Init(sd);
	m_spMarker->SetSca(m_markerScale);
	m_spMarker->SetPivot(m_SPRITE_PIVOT);
	m_spMarker->MainCameraView();
	m_spMarker->Mode_BillBord();
	m_spMarker->CameraScaleLock();
	m_spMarker->SetMulColor(m_COLOR);

	return true;
}


void TargetMarker::PreUpdate()
{

}

void TargetMarker::Update()
{
	auto eyePos = g_camera3D->GetPosition();
	auto vecEyeToParent = m_parentPos - eyePos;
	vecEyeToParent.y = 0.f;
	vecEyeToParent.Normalize();
	vecEyeToParent.Cross(Vector3::Up);
	auto right = vecEyeToParent;
	auto Pos = m_parentPos + m_markerPosOffset;// +right * m_flSpHpWidth / 2.f;
	m_spMarker->SetPos(Pos);
}

void TargetMarker::PostUpdate()
{

}


void TargetMarker::Render()
{

}

void TargetMarker::PostRender()
{

}
