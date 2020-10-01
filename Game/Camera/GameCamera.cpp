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
	m_position = m_targetPos + m_dist;
	auto cam = g_lockCamera3D.Get();
	cam->SetTarget(m_targetPos);
	cam->SetPosition(m_position);
}

