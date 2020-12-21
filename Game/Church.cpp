#include "stdafx.h"
#include "Church.h"


Church::Church()
{

}

Church::~Church()
{

}



void Church::Release()
{
	DeleteGO(m_model);
}

void Church::OnDestroy()
{
	Release();
}


void Church::Awake()
{

}

bool Church::Start()
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


void Church::PreUpdate()
{

}

void Church::Update()
{

}

void Church::PostUpdate()
{

}


void Church::Render()
{

}

void Church::PostRender()
{

}
