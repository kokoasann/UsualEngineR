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
	m_model->SetPosition(m_firstPosition);
	m_model->SetRotation(m_rotation);

	//closed
	m_animationMap.insert(std::make_pair(CLOSED, std::make_unique<CAnimationClip>()));
	m_animationMap.at(CLOSED)->Load("Assets/modelData/map_obj/anim/exit_T.tka");
	m_animationMap.at(CLOSED)->BuildKeyFramesAndAnimationEvents();
	m_animationMap.at(CLOSED)->SetLoopFlag(false);
	//opened
	m_animationMap.insert(std::make_pair(OPENED, std::make_unique<CAnimationClip>()));
	m_animationMap.at(OPENED)->Load("Assets/modelData/map_obj/anim/exit_open.tka");
	m_animationMap.at(OPENED)->BuildKeyFramesAndAnimationEvents();
	m_animationMap.at(OPENED)->SetLoopFlag(false);
	m_model->InitAnimation(m_animationMap, m_animationMap.size());

	m_model->Play(CLOSED);

	auto& wmat = m_model->GetModel().GetWorldMatrix();

	Vector3 forward;
	forward.x = wmat._31;
	forward.y = wmat._32;
	forward.z = wmat._33;
	forward.Normalize();
	m_openPosition = forward * m_openDoorSensorSensitivity;
	m_openPosition += m_lastPosition;

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

	//DebugPrintVector3(EDebugConsoloUser::WATA, player->GetPosition());

	m_appearTimer = min(m_appearTime, m_appearTimer +gameTime()->GetDeltaTime());
	pos.Lerp(m_appearTimer / m_appearTime, m_firstPosition, m_lastPosition);
	m_model->SetPosition(pos);

	if ((player->GetPosition() - m_openPosition).Length() <= m_openRange and !m_isOpened) {
		m_isOpened = true;
		m_model->Play(OPENED);
	}

	float dist = (player->GetPosition() - m_firstPosition).Length();
	if (dist <= m_range and !m_isChecked and m_isOpened){
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
