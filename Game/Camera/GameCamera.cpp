#include "stdafx.h"
#include "GameCamera.h"


GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}



void GameCamera::Release()
{

}

void GameCamera::OnDestroy()
{

}


void GameCamera::Awake()
{
	auto cam = g_lockCamera3D.Get();
	cam->SetTarget(m_targetPos);
	m_position = m_targetPos + m_position;
	m_dist = m_position - m_targetPos;
}

bool GameCamera::Start()
{
	return true;
}


void GameCamera::PreUpdate()
{

}

void GameCamera::Update()
{

}


void GameCamera::PostUpdate() {
	auto xf = g_pad[0]->GetRStickXF();
	auto yf = g_pad[0]->GetRStickYF();

	Quaternion rotY = Quaternion::Identity;
	Quaternion rotX = Quaternion::Identity;
	xf *= gameTime()->GetDeltaTime() * m_swivelSpeed;
	yf *= gameTime()->GetDeltaTime() * m_swivelSpeed;
	rotY.SetRotationDegY(xf);
	rotX.SetRotationDegX(yf);
	rotY.Multiply(rotX);
	rotY.Apply(m_dist);

	m_position = m_targetPos + m_dist;
	auto cam = g_lockCamera3D.Get();
	cam->SetTarget(m_targetPos);
	cam->SetPosition(m_position);
}

