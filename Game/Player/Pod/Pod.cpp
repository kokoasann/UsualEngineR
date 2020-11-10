#include "stdafx.h"
#include "Pod.h"
#include "../Player.h"

Pod::Pod()
{

}

Pod::~Pod()
{

}



void Pod::Release()
{
	DeleteGO(m_model);
}

void Pod::OnDestroy()
{
	Release();
}


void Pod::Awake()
{

}

bool Pod::Start()
{

	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test_criss.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);

	return true;
}


void Pod::PreUpdate()
{

}

void Pod::Update()
{
	auto p = mp_player->GetPosition() + m_distanceFromPlayer;
	m_pos = p;
	m_model->SetPosition(m_pos);
	m_model->SetRotation(m_rotation);
}

void Pod::PostUpdate()
{

}


void Pod::Render()
{

}

void Pod::PostRender()
{

}
