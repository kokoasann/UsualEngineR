#include "stdafx.h"
#include "Gun.h"


Gun::Gun()
{

}

Gun::~Gun()
{

}



void Gun::Release()
{
	DeleteGO(m_model);
}

void Gun::OnDestroy()
{
	Release();
}


void Gun::Awake()
{

}

bool Gun::Start()
{
	ModelInitData mid;
	m_model = NewGO<ModelRender>(0);
	mid.m_tkmFilePath = "Assets/modelData/m/Gun/Gun.tkm";
	mid.m_tksFilePath = "Assets/modelData/m/Gun/Gun.tks";
	//mid.m_upAxis = enUpAxisZ;
	m_model->Init(mid);

	return true;
}


void Gun::PreUpdate()
{

}

void Gun::Update()
{
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
	m_model->SetScale(m_scale);

	if (!m_isUsed) {
		m_model->SetActive(false);
	}
	else {
		m_model->SetActive(true);
	}
}

void Gun::PostUpdate()
{

}


void Gun::Render()
{

}

void Gun::PostRender()
{

}
