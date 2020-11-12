#include "stdafx.h"
#include "JetPack.h"


JetPack::JetPack()
{

}

JetPack::~JetPack()
{

}



void JetPack::Release()
{
	DeleteGO(m_model);
}

void JetPack::OnDestroy()
{
	Release();
}


void JetPack::Awake()
{

}

bool JetPack::Start()
{

	ModelInitData mid;
	m_model = NewGO<ModelRender>(0);
	mid.m_tkmFilePath = "Assets/modelData/m/Backpack/Backpack.tkm";
	mid.m_tksFilePath = "Assets/modelData/m/Backpack/Backpack.tks";
	//mid.m_upAxis = enUpAxisZ;
	m_model->Init(mid);

	return true;
}


void JetPack::PreUpdate()
{

}

void JetPack::Update()
{
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
}

void JetPack::PostUpdate()
{

}


void JetPack::Render()
{

}

void JetPack::PostRender()
{

}
