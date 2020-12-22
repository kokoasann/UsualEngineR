#include "stdafx.h"
#include "PowaPowa.h"


PowaPowa::PowaPowa()
{

}

PowaPowa::~PowaPowa()
{

}



void PowaPowa::Release()
{
	DeleteGO(m_model);

}

void PowaPowa::OnDestroy()
{
	Release();
}


void PowaPowa::Awake()
{

}

bool PowaPowa::Start()
{
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/AssistantMachine/am.tkm";
	mid.m_tksFilePath = "Assets/modelData/AssistantMachine/am.tks";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(m_scale);
	m_model->SetPosition(m_pos);
	m_model->SetRotation(m_rotation);

	return true;
}


void PowaPowa::PreUpdate()
{

}

void PowaPowa::Update()
{

}

void PowaPowa::PostUpdate()
{

}


void PowaPowa::Render()
{

}

void PowaPowa::PostRender()
{

}
