#include "stdafx.h"
#include "Shield.h"


Shield::Shield()
{

}

Shield::~Shield()
{

}



void Shield::Release()
{
	DeleteGO(m_model);
}

void Shield::OnDestroy()
{
	Release();
}


void Shield::Awake()
{

}

bool Shield::Start()
{

	ModelInitData mid;
	m_model = NewGO<ModelRender>(0);
	mid.m_tkmFilePath = "Assets/modelData/m/Shield/Shield.tkm";
	mid.m_tksFilePath = "Assets/modelData/m/Shield/Shield.tks";
	//mid.m_upAxis = enUpAxisZ;
	m_model->Init(mid);

	return true;
}


void Shield::PreUpdate()
{

}

void Shield::Update()
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

void Shield::PostUpdate()
{

}


void Shield::Render()
{

}

void Shield::PostRender()
{

}
