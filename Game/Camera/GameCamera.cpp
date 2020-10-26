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
	//auto posChecker = NewGO<PositionChecker>(0);
	//auto posChecker1 = NewGO<PositionChecker>(0);
	//auto posChecker2 = NewGO<PositionChecker>(0);
	//m_posCheckers.push_back(posChecker);
	//m_posCheckers.push_back(posChecker1);
	//m_posCheckers.push_back(posChecker2);

	//auto cam = g_lockCamera3D.Get();

	g_camera3D->SetTarget(m_targetPos);
	m_playerCameraPos = m_position = m_charaPos + m_position;
	//m_dist = m_position - m_charaPos;

	g_camera3D->SetTarget({ 0.0f, 0.0f, 0.0f });
	g_camera3D->SetPosition({ 0.0f, 3.0f, 35.f });

	m_toCameraPos.Set(0.0f, 3.0f, -15.f);
	/*
	m_springCamera.Init(
		g_camera3D,
		200.0f,
		false,
		5.0f
	);
	*/

	m_dist = m_toCameraPos;
	//m_springCamera.Update();

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

	/*

	target.y += 5.0f;
	Vector3 toCameraPosOld = m_toCameraPos;

	auto x = g_pad[0]->GetRStickXF();
	auto y = g_pad[0]->GetRStickYF();
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 7.0f * x);

	//qRot.Multiply(m_toCameraPos);
	qRot.Apply(m_toCameraPos);

	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 7.0f * y);
	//qRot.Multiply(m_toCameraPos);
	qRot.Apply(m_toCameraPos);

	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();

	if (toPosDir.y < -0.5f) {
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.8f) {
		m_toCameraPos = toCameraPosOld;
	}
	Vector3 pos = target + m_toCameraPos;

	m_playerCameraPos = pos;
	m_playerCameraTargetPos = target;

	//CalcEnemyCamera();
	//CalcPlayerCamera();
	
	if (m_state == State::enPlayerCamera) {

		m_springCamera.SetTarget(m_playerCameraTargetPos);
		m_springCamera.SetPosition(m_playerCameraPos);
		//printf("player camera\n");
	}
	else {
		//printf("enemy camera\n");
		auto vecToCamearFromChara = m_springCamera.GetPosition() - m_charaPos;
		auto vecToCameraFromEnemy = m_springCamera.GetPosition() - mp_enemy->GetPosition();
		vecToCameraFromEnemy.y = 0.f;
		vecToCameraFromEnemy.Normalize();
		vecToCameraFromEnemy.Scale(m_toCameraPos.Length());
		//vecToCameraFromEnemy.Scale(-vecToCamearFromChara.Length());
		//auto npos = m_charaPos + vecToCameraFromEnemy;

		m_springCamera.SetTarget(mp_enemy->GetPosition());
		 m_enemyCameraPos = mp_player->GetPosition() + vecToCameraFromEnemy;
		m_springCamera.SetPosition(m_enemyCameraPos);
	}
	m_springCamera.Update();

	//CalcCameraPosition();


	//m_charaPos = mp_player->GetPosition();

	//Vector3 target = mp_player->GetPosition();

	//target.y += 5.0f;
	//Vector3 toCameraPosOld = m_toCameraPos;
	//auto x = g_pad[0]->GetRStickXF();
	//auto y = g_pad[0]->GetRStickYF();
	//Quaternion qRot;
	//qRot.SetRotationDeg(Vector3::AxisY, 7.0f * x);
	//qRot.Apply(m_toCameraPos);
	////qRot.Mul(m_toCameraPos);

	//Vector3 axisX;
	//axisX.Cross(Vector3::AxisY, m_toCameraPos);
	//axisX.Normalize();
	//qRot.SetRotationDeg(axisX, 7.0f * y);
	//qRot.Apply(m_toCameraPos);
	////qRot.Multiply(m_toCameraPos);

	//Vector3 toPosDir = m_toCameraPos;
	//toPosDir.Normalize();

	//if (toPosDir.y < -0.5f) {
	//	m_toCameraPos = toCameraPosOld;
	//}
	//else if (toPosDir.y > 0.8f) {
	//	m_toCameraPos = toCameraPosOld;
	//}

	//Vector3 pos = target + m_toCameraPos;

	//m_springCamera.SetTarget(target);
	//m_springCamera.SetPosition({ 0,100,1000 });
	//m_springCamera.Update();
	*/
}


void GameCamera::PostUpdate() {

	{
		//auto forward = mp_player->GetForward();
		auto vecCtoECP = m_enemyCameraPos - mp_player->GetPosition();
		static float cameraDist = vecCtoECP.Length();
		//forward.Scale(30.f);
		//m_posCheckers.at(0)->SetPos(vecCtoECP + mp_player->GetPosition());
		auto vecCtoPCP = m_playerCameraPos - mp_player->GetPosition();
		//auto backward = m_playerCameraPos;
		//backward.Scale(-1.f);
		//m_posCheckers.at(1)->SetPos(vecCtoPCP + mp_player->GetPosition());

		float dist_cecp = vecCtoECP.Length();
		float dist_cpcp = vecCtoPCP.Length();

		vecCtoECP.Normalize();
		vecCtoPCP.Normalize();

	Matrix mat1, mat2;
	mat1.MakeLookAt(Vector3::Zero, vecCtoECP, Vector3::Up);
	mat2.MakeLookAt(Vector3::Zero, vecCtoPCP, Vector3::Up);
	Quaternion frot(mat1);
	Quaternion brot(mat2);
	//Quaternion sl;

	//static float ratio = 0.0f;
		/*
		if (g_pad[0]->IsPress(enButtonLeft)) {
			m_cameraChangeRatio = min(1.f, m_cameraChangeRatio += 0.02f);
		}
		if (g_pad[0]->IsPress(enButtonRight)) {
			m_cameraChangeRatio = max(0.f, m_cameraChangeRatio -= 0.02f);
		}
		printf("Ratio : %f\n", m_cameraChangeRatio);
		*/

		//ratio 0 == enemy camera, ratio 1 == player camera
		printf("Ratio : %f\n", m_cameraChangeRatio);

		Quaternion q1;
		//auto t = UER::Dot(from, to);
		//auto t = vecCtoECP.Dot(vecCtoPCP);
		//q1.SetRotation(Vector3::AxisY, acosf(t));
		q1.SetRotation(vecCtoECP, vecCtoPCP);
		Quaternion sl;
		//sl.Slerp(m_cameraChangeRatio, vecCtoECP, q1);
		sl.Slerp(m_cameraChangeRatio, Quaternion::Identity, q1);
		//Matrix mat3;
		//mat3.MakeLookAt(Vector3::Zero, m_enemyCameraPos, Vector3::Up);
		//Quaternion ecrot(mat3);
		//sl.Slerp(ratio, frot, brot);
		//Vector3 slpos = vecCtoECP;
		Vector3 slpos = m_enemyCameraPos - mp_player->GetPosition();
		sl.Apply(slpos);
		//slpos.Scale(30.f);


		//auto scale = 1  ratio * dist_cecp / dist_cpcp;

		auto scaleRatio = dist_cpcp / dist_cecp;

		auto scale = Math::Lerp(m_cameraChangeRatio, 1.f, scaleRatio);
		slpos.Scale(scale);

		m_position = slpos + m_charaPos;
		//m_posCheckers.at(2)->SetPos(m_position);

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

	return;

	//auto cam = g_lockCamera3D.Get();

	//CalcEnemyCamera();
	//CalcPlayerCamera();

	//CalcCameraPosition();


	////
	//auto vecCharaToEnemy = mp_enemy->GetPosition() - m_charaPos;
	//Quaternion rot = Quaternion::Identity;
	//auto thetaY = atan2(vecCharaToEnemy.x, vecCharaToEnemy.z);
	////thetaY = thetaY * (180.f / Math::PI);
	////rot.SetRotationDegY(thetaY);
	//auto forward = cam->GetForward();
	//auto thetaF = atan2(forward.x, forward.z);
	//auto diffTheta = thetaY - thetaF;
	//rot.SetRotationY(diffTheta);

	//rot.Apply(m_targetPos);
	////printf("Diff theta : %f\n", thetaY - thetaF);
	////

	//cam->SetTarget(m_targetPos);
	//cam->SetPosition(m_position);

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
	//m_charaPos += vecRight * charaSlideParam;

	auto tarp = m_targetPos;
	tarp -= vecRight * targetSlideParam;
	tarp = m_charaPos + vecRight * charaSlideParam;;


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


	if (m_cameraChangeRatio == 0.f) {
		auto pf = mp_player->GetForward();
		pf.Scale(-1.f * m_toCameraPos.Length());
		pf.y += cameraHeight;
		m_playerCameraPos = m_charaPos + pf;
		m_dist = m_playerCameraPos - mp_player->GetPosition();
	}

	m_old = m_dist;

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

		//


		//p->SetRotation(rot);
		//

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
	//if (g_pad[0]->IsTrigger(enButtonLB3)) {
	if (g_pad[0]->IsTrigger(enButtonX)) {
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