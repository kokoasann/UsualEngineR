#include "stdafx.h"
#include "GameCamera.h"
#include "../Player/Player.h"
#include "../Enemy/IEnemy.h"

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
	m_playerCameraPos = m_position = m_charaPos + m_position;
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

	UpdateState();

	m_charaPos = mp_player->GetPosition();

	CalcEnemyCamera();
	CalcPlayerCamera();

	CalcCameraPosition();

	cam->SetTarget(m_targetPos);
	cam->SetPosition(m_position);

	m_old = m_dist;
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


	//TODO : ’l‚Í“¯ŽžŒvŽZ‚µ‚ÄUpdate‚Å‚Ç‚¿‚ç‚ðŽg‚¤‚©”»’f‚·‚é‚©•âŠ®‚·‚é
	m_enemyCameraPos = ecPos;
	m_enemyCameraTargetPos = tarp;

	//cam->SetTarget(tarp);
	//cam->SetPosition(m_position);

}

void GameCamera::CalcPlayerCamera() {
	//auto cam = g_lockCamera3D.Get();
	auto xf = g_pad[0]->GetRStickXF();
	auto yf = g_pad[0]->GetRStickYF();
	//
	static const float upperLimit = 0.7f;
	static const float lowerLimit = -0.5f;

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

	//cam->SetTarget(m_charaPos + m_furtherTargetHeight);
	//cam->SetPosition(m_position);
}

void GameCamera::CalcCameraPosition() {
	if (m_state == State::enEnemyCamera) {

		auto& epos = mp_enemy->GetPosition();


		float xdist = 1.f;
		float ydist = 1.f;

		static const float lowerLimit = -0.5f;
		auto vecCharaToCamera = m_playerCameraPos - m_charaPos;
		Quaternion rotY = Quaternion::Identity;
		Quaternion rotX = Quaternion::Identity;
		xdist *= gameTime()->GetDeltaTime() * m_swivelSpeed;
		ydist *= gameTime()->GetDeltaTime() * m_swivelSpeed;
		auto x = vecCharaToCamera;
		x.Normalize();
		rotY.SetRotationDegY(xdist);
		auto vecRight = vecCharaToCamera;
		vecRight.y = 0.f;
		vecRight.Cross({ 0,1,0 });
		vecRight.Normalize();
		rotX.SetRotationDeg(vecRight, ydist);
		rotY.Multiply(rotX);
		rotY.Apply(m_dist);

		auto nd = m_dist;
		nd.Normalize();

		//if (nd.y > upperLimit or nd.y < lowerLimit)
//			m_dist = m_old;

		m_position = m_charaPos + m_dist;
		m_targetPos = m_charaPos + m_furtherTargetHeight;

		/*
		Quaternion res, rot1, rot2;
		res = rot1 = rot2 = Quaternion::Identity;

		auto vecToEnemyCameraFromChara = m_enemyCameraPos - m_charaPos;
		auto vecToPlayerCmeraFromChara = m_playerCameraPos - m_charaPos;

		auto vecToEnemyFromCamera = m_enemyCameraTargetPos - m_position;
		auto vecToCurrentTargetPosFromCamera = m_targetPos - m_position;

		rot1.SetRotation(vecToCurrentTargetPosFromCamera, vecToEnemyFromCamera);
		res.Slerp(0, Quaternion::Identity, rot1);
		res.Apply(vecToCurrentTargetPosFromCamera);

		auto vecToCameraFromChara = m_position - m_charaPos;
		auto dist = vecToCameraFromChara.Length();

		m_targetPos = vecToCurrentTargetPosFromCamera;
		m_position = m_charaPos + (vecToCurrentTargetPosFromCamera * -1 * dist);
		*/

		//Quaternion enemy camera to player camera
		/*
		static float timer = 0.f;
		auto delta = gameTime()->GetDeltaTime();
		auto ratio = min(1.f, timer);
		rot1.SetRotation(vecToEnemyCameraFromChara, vecToPlayerCmeraFromChara);
		res.Slerp(0, Quaternion::Identity, rot1);
		res.Apply(vecToEnemyCameraFromChara);

		m_position = m_charaPos + vecToEnemyCameraFromChara;
		m_targetPos = m_enemyCameraTargetPos;
		//cam->SetTarget(m_enemyCameraTargetPos);
		//cam->SetPosition(m_enemyCameraPos);
		*/
	}

	if (m_state == State::enPlayerCamera) {
		m_position = m_playerCameraPos;
		m_targetPos = m_playerCameraTargetPos;
		//cam->SetTarget(m_playerCameraTargetPos);
		//cam->SetPosition(m_playerCameraPos);
	}

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