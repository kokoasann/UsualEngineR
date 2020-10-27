#include "stdafx.h"
#include "GameCamera.h"
#include "../Player/Player.h"
#include "../Enemy/IEnemy.h"
#include "../PositionChecker.h"

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
	g_camera3D->SetTarget(m_targetPos);
	m_playerCameraPos = m_position = m_charaPos + m_position;
	g_camera3D->SetTarget({ 0.0f, 0.0f, 0.0f });
	g_camera3D->SetPosition(m_position);
	m_toCameraPos.Set(0.0f, 3.0f, -15.f);
	m_dist = m_toCameraPos;
	
	DebugLogVec3(g_camera3D->GetPosition());

	return true;
}


void GameCamera::PreUpdate()
{

}

void GameCamera::Update()
{
	Vector3 target = mp_player->GetPosition();
	m_charaPos = mp_player->GetPosition();

	UpdateState();
	CalcEnemyCamera();
	CalcPlayerCamera();
}


void GameCamera::PostUpdate() {

	{
		auto vecCtoECP = m_enemyCameraPos - mp_player->GetPosition();
		static float cameraDist = vecCtoECP.Length();
		auto vecCtoPCP = m_playerCameraPos - mp_player->GetPosition();

		float dist_cecp = vecCtoECP.Length();
		float dist_cpcp = vecCtoPCP.Length();

		vecCtoECP.Normalize();
		vecCtoPCP.Normalize();

		Matrix mat1, mat2;
		mat1.MakeLookAt(Vector3::Zero, vecCtoECP, Vector3::Up);
		mat2.MakeLookAt(Vector3::Zero, vecCtoPCP, Vector3::Up);
		Quaternion frot(mat1);
		Quaternion brot(mat2);

		Quaternion q1;
		q1.SetRotation(vecCtoECP, vecCtoPCP);
		Quaternion sl;
		sl.Slerp(m_cameraChangeRatio, Quaternion::Identity, q1);
		Vector3 slpos = m_enemyCameraPos - mp_player->GetPosition();
		sl.Apply(slpos);
		auto scaleRatio = dist_cpcp / dist_cecp;
		auto scale = Math::Lerp(m_cameraChangeRatio, 1.f, scaleRatio);
		slpos.Scale(scale);
		m_position = slpos + m_charaPos;

		g_camera3D->SetPosition(m_position);
		auto tar = Math::Lerp(m_cameraChangeRatio, m_enemyCameraTargetPos, m_playerCameraTargetPos);
		g_camera3D->SetTarget(tar);

		if (m_state == State::enPlayerCamera) {
			m_cameraChangeRatio = min(1.f, m_cameraChangeRatio += m_transitionSpeed);
		}
		else {
			m_cameraChangeRatio = max(0.f, m_cameraChangeRatio -= m_transitionSpeed);
		}
	}
}


void GameCamera::CalcEnemyCamera() {
	static float distParam = 15.f;
	static float cameraHeight = 5.f;
	static float charaSlideParam = 10.f;
	static float targetSlideParam = 30.f;

	Vector3 ecPos;

	auto vecTargetToChara = m_charaPos - m_targetPos;
	auto length = vecTargetToChara.Length();
	vecTargetToChara.Normalize();
	ecPos = m_targetPos + vecTargetToChara * (length + distParam);
	ecPos.y += cameraHeight;

	auto vecRight = vecTargetToChara;
	vecRight.y = 0.f;
	vecRight.Cross({ 0,1,0 });
	vecRight.Normalize();
	ecPos += vecRight * charaSlideParam;

	auto tarp = m_targetPos;
	tarp -= vecRight * targetSlideParam;
	tarp = m_charaPos + vecRight * charaSlideParam;;

	m_enemyCameraPos = ecPos;
	m_enemyCameraTargetPos = tarp;
}

void GameCamera::CalcPlayerCamera() {
	auto xf = g_pad[0]->GetRStickXF();
	auto yf = g_pad[0]->GetRStickYF();

	static const float upperLimit = 0.7f;
	static const float lowerLimit = -0.5f;
	static float cameraHeight = 5.f;

	auto vecCharaToCamera = m_playerCameraPos - m_charaPos;

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

	m_playerCameraPos = m_charaPos + m_dist;
	m_playerCameraTargetPos = m_charaPos + m_furtherTargetHeight;


	//エネミーカメラに完全に切り替わったらプレイヤーカメラの位置を計算
	if (m_cameraChangeRatio == 0.f) {
		//auto pf = mp_player->GetForward();
		//pf.Scale(-1.f * m_toCameraPos.Length());
		//pf.y += cameraHeight;
		//m_playerCameraPos = m_charaPos + pf;

		auto cameraForward = g_camera3D->GetForward();
		cameraForward.y = 0.f;
		cameraForward.Normalize();
		cameraForward.Scale(-1 * m_toCameraPos.Length());
		cameraForward.y += cameraHeight;
		m_playerCameraPos = m_charaPos + cameraForward;

		m_dist = m_playerCameraPos - mp_player->GetPosition();
	}

	m_old = m_dist;

	DebugLogVec3(g_camera3D->GetPosition());

}


void GameCamera::UpdateState() {
	if (g_pad[0]->IsTrigger(enButtonLB3)) {
	//if (g_pad[0]->IsTrigger(enButtonX)) {
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