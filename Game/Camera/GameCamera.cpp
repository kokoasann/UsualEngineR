#include "stdafx.h"
#include "GameCamera.h"
#include "../Player/Player.h"

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
}

bool GameCamera::Start()
{
	auto cam = g_lockCamera3D.Get();
	cam->SetTarget(m_targetPos);
	m_position = m_charaPos + m_position;
	m_dist = m_position - m_charaPos;

	return true;
}


void GameCamera::PreUpdate()
{

}

void GameCamera::Update()
{

}


void GameCamera::PostUpdate() {

	m_charaPos = mp_player->GetPosition();

	if (m_state == State::enEnemyCamera) {
		CalcEnemyCamera();
	}

	if (m_state == State::enPlayerCamera) {
		CalcPlayerCamera();
	}
	m_old = m_dist;
}


void GameCamera::CalcEnemyCamera() {
	static float distParam = 15.f;
	static float cameraHeight = 5.f;
	static float charaSlideParam = 10.f;
	static float targetSlideParam = 30.f;

	auto vecTargetToChara = m_charaPos - m_targetPos;
	auto length = vecTargetToChara.Length();
	vecTargetToChara.Normalize();
	m_position = m_targetPos + vecTargetToChara * (length + distParam);
	m_position.y += cameraHeight;

	auto vecRight = vecTargetToChara;
	vecRight.y = 0.f;
	vecRight.Cross({ 0,1,0 });
	vecRight.Normalize();
	m_position += vecRight * charaSlideParam;

	auto tarp = m_targetPos;
	tarp -= vecRight * targetSlideParam;

	auto cam = g_lockCamera3D.Get();
	cam->SetTarget(tarp);
	cam->SetPosition(m_position);
}

void GameCamera::CalcPlayerCamera() {
	auto cam = g_lockCamera3D.Get();
	auto xf = g_pad[0]->GetRStickXF();
	auto yf = g_pad[0]->GetRStickYF();
	//
	static const float upperLimit = 0.7f;
	static const float lowerLimit = -0.5f;

	auto vecCharaToCamera = m_position - m_charaPos;

	Quaternion rotY = Quaternion::Identity;
	Quaternion rotX = Quaternion::Identity;
	xf *= gameTime()->GetDeltaTime() * m_swivelSpeed;
	yf *= gameTime()->GetDeltaTime() * m_swivelSpeed;

	auto x = vecCharaToCamera;
	x.Normalize();

	rotY.SetRotationDegY(xf);
	auto vecRight = vecCharaToCamera;
	vecRight.y = 0.f;
	vecRight.Cross({ 0,1,0 });
	vecRight.Normalize();
	rotX.SetRotationDeg(vecRight, yf);
	rotY.Multiply(rotX);
	rotY.Apply(m_dist);

	auto nd = m_dist;
	nd.Normalize();

	if (nd.y > upperLimit or nd.y < lowerLimit)
		m_dist = m_old;


	m_position = m_charaPos + m_dist;

	cam->SetTarget(m_charaPos + m_furtherTargetHeight);
	cam->SetPosition(m_position);
}

void GameCamera::UpdateState() {
	if (g_pad[0]->IsTrigger(enButtonLB3)) {
		if (m_state == State::enEnemyCamera) {
			m_state = State::enPlayerCamera;
		}
		else {
			if (mp_player->GetTargetEnemy() != nullptr)
				m_state = State::enEnemyCamera;
		}
	}

	if (mp_player->GetTargetEnemy() == nullptr)
		m_state = State::enPlayerCamera;
}