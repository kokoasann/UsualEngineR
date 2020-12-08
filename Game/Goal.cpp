#include "stdafx.h"
#include "Goal.h"
#include "GameManager.h"
#include "Player/Player.h"
#include "Game.h"

Goal::Goal()
{

}

Goal::~Goal()
{

}



void Goal::Release()
{

}

void Goal::OnDestroy()
{

}


void Goal::Awake()
{

}

bool Goal::Start()
{
	return true;
}


void Goal::PreUpdate()
{

}

void Goal::Update()
{

	float dist = (GameManager::GetInstance().m_player->GetPosition() - m_position).Length();
	if (dist <= m_range and !m_isChecked){
		GameManager::GetInstance().m_gameScene->OnGoal();
		m_isChecked = true;
	}

}

void Goal::PostUpdate()
{

}


void Goal::Render()
{

}

void Goal::PostRender()
{

}
