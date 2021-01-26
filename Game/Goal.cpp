#include "stdafx.h"
#include "Goal.h"
#include "GameManager.h"
#include "Player/Player.h"
#include "Game.h"
#include "Camera/GameCamera.h"
#include "Player/Pod/Pod.h"

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
	Quaternion rot = Quaternion::Identity;
	rot.SetRotationDegY(90.f);
	m_rotation.Multiply(rot);
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

	m_forward.x = wmat._31;
	m_forward.y = wmat._32;
	m_forward.z = wmat._33;
	rot.Apply(m_forward);
	m_forward.Normalize();
	m_openPosition = m_forward * m_openDoorSensorSensitivity;
	m_openPosition += m_lastPosition;

	//Physics
	m_physicsStaticObject.CreateMeshObject(m_model->GetModel(), m_lastPosition, m_model->GetRotation(), m_model->GetScale());

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

		auto cam = GameManager::GetInstance().m_camera;
		auto tar = m_lastPosition;
		tar.y += 20.f;

		auto camEndPos = m_lastPosition + m_forward * 60.f;
		camEndPos.y += 20.f;
		auto sec = 2.f;
		auto interval = 2.f;

		cam->Perform(
			camEndPos, camEndPos,
			tar, tar, sec, interval
		);

		m_model->Play(OPENED);

		//Player
		Vector3 playerPosBegin = m_forward * 35.f;
		playerPosBegin.y = 9.f;
		Quaternion rot = Quaternion::Identity;
		rot.SetRotationDegY(-90.f);
		const float walkSpeed = 2.5f;
		auto vel = m_forward * -1.f * walkSpeed;
		vel.y = -1.f;

		player->StopThrusters();
		player->PlayAnimation(Player::EnAnimation::enWalk);
		auto animSpeed= player->GetModelRender().GetAnimPlaySpeed();
		player->GetModelRender().SetAnimPlaySpeed(animSpeed * 0.5);
		player->MovePerformance(playerPosBegin, vel, rot);

		//Pod
		const Vector3 distanceFromPlayer = { 5.f,10.f,-5.f };
		player->GetPod()->SetPosition(player->GetPosition() + distanceFromPlayer);
		player->GetPod()->SetRotation(rot);

	}


	if (m_isOpened) {
		auto delta = gameTime()->GetDeltaTime();
		m_performanceTimer += delta;
		//update chara's pos
		auto& charaCon = player->GetCharaCon();
		charaCon.Execute(delta, player->m_velocity);
		player->SetPosition(charaCon.GetPosition());
		//
		if (m_performanceTimer >= m_sceneTransitionTime and !m_isChecked) {
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


