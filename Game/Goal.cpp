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
	DeleteGO(m_model);
}

void Goal::OnDestroy()
{
	Release();
}


void Goal::Awake()
{

}

bool Goal::Start()
{

	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/map_obj/exit.tkm";
	mid.m_tksFilePath = "Assets/modelData/map_obj/exit.tks";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(m_scale);
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);

	return true;
}


void Goal::PreUpdate()
{

}

void Goal::Update()
{
	auto player = GameManager::GetInstance().GetPlayer();
	if (player == nullptr) return;

	Vector3 pos = Vector3::Zero;

	m_appearTimer = min(m_appearTime, m_appearTimer +gameTime()->GetDeltaTime());
	pos.Lerp(m_appearTimer / m_appearTime, m_position, Vector3::Zero);
	m_model->SetPosition(pos);

	float dist = (player->GetPosition() - m_position).Length();
	if (dist <= m_range and !m_isChecked){
		if (g_pad[0]->IsTrigger(enButtonB)) {
			GameManager::GetInstance().m_gameScene->OnGoal();
			m_isChecked = true;
		}
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
