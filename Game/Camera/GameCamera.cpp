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

	auto cam = g_lockCamera3D.Get();
	auto xf = g_pad[0]->GetRStickXF();
	auto yf = g_pad[0]->GetRStickYF();

	if (g_pad[0]->IsPress(enButtonY)) {
		m_state = enEnemyCamera;
	}
	else {
		m_state = enPlayerCamera;
	}

	if (m_state == enEnemyCamera) {
		static float distParam = 15.f;
		static float cameraHeight = 5.f;
		static float slideParam = 10.f;

		auto vecTargetToChara = m_charaPos - m_targetPos;
		auto length = vecTargetToChara.Length();
		vecTargetToChara.Normalize();
		m_position = m_targetPos + vecTargetToChara * (length + distParam);
		m_position.y += cameraHeight;

		auto vecRight = vecTargetToChara;
		vecRight.y = 0.f;
		vecRight.Cross({ 0,1,0 });
		vecRight.Normalize();
		vecRight *= slideParam;

		m_position += vecRight;

		cam->SetTarget(m_targetPos);
	}

	if (m_state == enPlayerCamera) {

		auto vecCharaToCamera = m_position - m_charaPos;

		Quaternion rotY = Quaternion::Identity;
		Quaternion rotX = Quaternion::Identity;
		xf *= gameTime()->GetDeltaTime() * m_swivelSpeed;
		yf *= gameTime()->GetDeltaTime() * m_swivelSpeed;

		auto x = vecCharaToCamera;
		x.Normalize();
		printf("y : %f\n", x.y);
		/*
		auto h = abs(m_charaPos.y - m_position.y);
		const float limit = 10.f;
		printf("H : %f\n", h);
		if ((h >= limit && yf > 0) or (h <= -limit && yf <0)) {

		}
		*/

		rotY.SetRotationDegY(xf);
		auto vecRight = vecCharaToCamera;
		vecRight.y = 0.f;
		vecRight.Cross({ 0,1,0 });
		vecRight.Normalize();
		rotX.SetRotationDeg(vecRight, yf);
		rotY.Multiply(rotX);
		rotY.Apply(m_dist);

		m_position = m_charaPos + m_dist;

		cam->SetTarget(m_charaPos);
	}

	cam->SetPosition(m_position);
	m_old = m_dist;
}
