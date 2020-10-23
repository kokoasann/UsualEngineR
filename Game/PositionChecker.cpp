#include "stdafx.h"
#include "PositionChecker.h"


PositionChecker::PositionChecker()
{

}

PositionChecker::~PositionChecker()
{

}



void PositionChecker::Release()
{
	DeleteGO(m_model);
}

void PositionChecker::OnDestroy()
{
	Release();
}



bool PositionChecker::Start()
{
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_upAxis = enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(m_scale);

	return true;
}



void PositionChecker::Update()
{
	m_model->SetPosition(m_position);
}




