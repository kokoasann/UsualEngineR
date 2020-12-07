#include "stdafx.h"
#include "GameCamera.h"
#include "../Player/Player.h"
#include "../Enemy/IEnemy.h"
#include "../PositionChecker.h"
#include "Enemy/EnemyManager.h"
#include <limits>
#include "../GameManager.h"
#include "../GameSceneMenu.h"
#include "Game.h"
//#define POS_CHECK

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

	this->Reset();

	m_cameraCollisionSolver.Init(m_sphereCollisionRadius);

	auto& enemies = EnemyManager::GetEnemyManager().GetEnemies();
	if (enemies.size() == 0) {
		EnemyManager::GetEnemyManager().SetTargetEnemy(nullptr);
	}
	else {
		m_enemyCameraCurrentTargetPos = m_enemyCameraNextTargetPos = enemies.at(m_targetEnemyNo)->GetPosition();
	}

#ifdef POS_CHECK
	m_posChecker = NewGO<PositionChecker>(0);
	m_posCheckerL = NewGO<PositionChecker>(0);
	m_posCheckerR = NewGO<PositionChecker>(0);
#endif // POS_CHECK


	return true;
}

void GameCamera::PreUpdate()
{

}

void GameCamera::Update()
{
	if (GameManager::GetInstance().m_player == nullptr) return;

	m_charaPos = GameManager::GetInstance().m_player->GetPosition();

#ifdef POS_CHECK

	int centerIndex, leftIndex, rightIndex;
	std::tie(centerIndex, leftIndex, rightIndex) = GetTargetEnemyIndexes();

		if (centerIndex != -1) {
			const auto& chekcerPos = enemies.at(centerIndex)->GetPosition();
			m_posChecker->SetPos(chekcerPos);
		}
		if (leftIndex != -1) {
			const auto& chekcerPosL = enemies.at(leftIndex)->GetPosition();
			m_posCheckerL->SetPos(chekcerPosL);
		}
		if (rightIndex != -1) {
			const auto& chekcerPosR = enemies.at(rightIndex)->GetPosition();
			m_posCheckerR->SetPos(chekcerPosR);
		}
	//m_posChecker->SetPos(enemies.at(forwardEnemyIndex)->GetPosition());
#endif // POS_CHECK

	if (m_state == State::enSlerpPerformanceCamera) {
		CalcSlerpPerformanceCamera();
	}
	else if (m_state == State::enLerpPerformanceCamera) {
		CalcLerpPerformanceCamera();
	}
	else {
		UpdateState();
		CalcTarget();
		CalcEnemyCamera();
		CalcPlayerCamera();
	}

	//DEBUG
	//if (g_pad[0]->IsTrigger(enButtonB)) {
	//	Perform(m_playerCameraPos, m_enemyCameraPos, m_playerCameraTargetPos, m_enemyCameraTargetPos, m_charaPos, 0.001f);
	//}
}

void GameCamera::PostUpdate() {

	if (GameManager::GetInstance().m_player == nullptr) return;
	if (m_state == State::enLerpPerformanceCamera) return;
	if (m_state == State::enSlerpPerformanceCamera) return;

	const float PlayerCameraRatio = 1.f;
	const float EnemyCameraRatio = 0.f;

	if (m_cameraChangeRatio == PlayerCameraRatio) {
		m_position = m_playerCameraPos;
	}
	else if (m_cameraChangeRatio == EnemyCameraRatio) {
		m_position = m_enemyCameraPos;
	}
	else {
		//補完中
		auto vecCtoECP = m_enemyCameraPos - GameManager::GetInstance().m_player->GetPosition();
		static float cameraDist = vecCtoECP.Length();
		auto vecCtoPCP = m_playerCameraPos - GameManager::GetInstance().m_player->GetPosition();

		float dist_cecp = vecCtoECP.Length();
		float dist_cpcp = vecCtoPCP.Length();

		vecCtoECP.Normalize();
		vecCtoPCP.Normalize();

		Quaternion q1;
		q1.SetRotation(vecCtoECP, vecCtoPCP);
		Quaternion sl;
		sl.Slerp(m_cameraChangeRatio, Quaternion::Identity, q1);
		Vector3 slpos = m_enemyCameraPos - GameManager::GetInstance().m_player->GetPosition();
		sl.Apply(slpos);
		auto scaleRatio = dist_cpcp / dist_cecp;
		auto scale = Math::Lerp(m_cameraChangeRatio, 1.f, scaleRatio);
		slpos.Scale(scale);

		m_position = slpos + m_charaPos;
	}

	g_camera3D->SetPosition(m_position);
	auto tar = Math::Lerp(m_cameraChangeRatio, m_enemyCameraTargetPos, m_playerCameraTargetPos);
	g_camera3D->SetTarget(tar);

	//カメラ当たり判定.
	Vector3 result;
	auto flag = m_cameraCollisionSolver.Execute(
		result,
		g_camera3D->GetPosition(),
		g_camera3D->GetTarget()
	);

	//急に距離が変わっていないか調べる.
	float dlt = 0.f;
	float dist = (result - tar).Length();
	dlt = m_oldDist - dist;
	//カメラが荒ぶるようなリザルトの場合は無視.
	if (abs(dlt) <= 10.f and dist >= 0.1f) {
		m_oldDist = dist;
		g_camera3D->SetPosition(result);
	}
	//printf("delta %f / dist %f\n", dlt,dist);

	if (m_state == State::enPlayerCamera) {
		m_cameraChangeRatio = min(1.f, m_cameraChangeRatio += m_transitionSpeed * gameTime()->GetDeltaTime());
	}
	else {
		m_cameraChangeRatio = max(0.f, m_cameraChangeRatio -= m_transitionSpeed * gameTime()->GetDeltaTime());
	}
	//DebugPrintVector3(TO_INT(EDebugConsoloUser::WATA), m_position);
}

void GameCamera::CalcTarget() {
	auto& enemies = EnemyManager::GetEnemyManager().GetEnemies();

	if (enemies.size() == 0) {
		EnemyManager::GetEnemyManager().SetTargetEnemy(nullptr);
	}
	else {
		//ターゲット切替.

		if (m_state != State::enEnemyCamera) return;

		const float changeTargetVal = 0.7f;
		auto rxf = g_pad[0]->GetRStickXF();

		if (enemyTargetChangeTime >= 0.3f) {

			int centerIndex, leftIndex, rightIndex;

			bool isChangeTarget = false;

			//right
			if (rxf >= changeTargetVal) {
				isChangeTarget = true;
				std::tie(centerIndex, leftIndex, rightIndex) = GetTargetEnemyIndexes();
				if (rightIndex != -1) {
					m_targetEnemyNo = rightIndex;
				}
				else if (centerIndex != -1) {
					m_targetEnemyNo = centerIndex;
				}
			}

			//left
			if (rxf <= -changeTargetVal) {
				isChangeTarget = true;
				std::tie(centerIndex, leftIndex, rightIndex) = GetTargetEnemyIndexes();
				if (leftIndex != -1) {
					m_targetEnemyNo = leftIndex;
				}
				else if (centerIndex != -1) {
					m_targetEnemyNo = centerIndex;
				}
			}

			//center
			if (g_pad[0]->IsTrigger(enButtonRB3)) {
				isChangeTarget = true;
				std::tie(centerIndex, leftIndex, rightIndex) = GetTargetEnemyIndexes();
				if (centerIndex != -1) {
					m_targetEnemyNo = centerIndex;
					m_enemyCameraCurrentTargetPos = m_enemyCameraNextTargetPos = enemies.at(m_targetEnemyNo)->GetPosition();
				}
			}

			if (m_targetEnemyNo != -1 and isChangeTarget) {
				enemyTargetChangeTime = 0.f;
				m_enemyCameraNextTargetPos = enemies.at(m_targetEnemyNo)->GetPosition();
				EnemyManager::GetEnemyManager().SetTargetEnemy(enemies.at(m_targetEnemyNo));
			}
		}
	}
}

void GameCamera::CalcEnemyCamera() {
	static float distParam = 30.f;
	static float cameraHeight = 8.f;
	static float charaSlideParam = 5.f;
	static float targetSlideParam = 30.f;

	Vector3 ecPos;

	auto targettingEnemy = EnemyManager::GetEnemyManager().GetTargettingEnemy();
	if (targettingEnemy != nullptr) {
		m_targetPos = targettingEnemy->GetPosition();
		m_enemyCameraNextTargetPos = targettingEnemy->GetPosition();
	}
	else {
		m_state = State::enPlayerCamera;
		m_targetEnemyNo = -1;
	}

	//ターゲットの位置を切替先まで補完する.
	auto vecCurrentToNext = m_enemyCameraNextTargetPos - m_enemyCameraCurrentTargetPos;
	auto lengthToNextTargetPos = vecCurrentToNext.Length();
	vecCurrentToNext.Normalize();
	auto currentLength = lengthToNextTargetPos * enemyTargetChangeTime;
	m_enemyCameraCurrentTargetPos += vecCurrentToNext * currentLength;
	enemyTargetChangeTime += gameTime()->GetDeltaTime();
	enemyTargetChangeTime = min(gameTime()->GetDeltaTime() + enemyTargetChangeTime, 1.f);
	m_targetPos = m_enemyCameraCurrentTargetPos;

	//キャラとターゲットが一直線に重なって見えるようなカメラの位置を計算.
	auto vecTargetToChara = m_charaPos - m_targetPos;
	auto length = vecTargetToChara.Length();
	vecTargetToChara.Normalize();
	ecPos = m_targetPos + vecTargetToChara * (length + distParam);
	//カメラの位置を上げる.
	ecPos.y += cameraHeight;

	//カメラを右側にずらす.
	auto vecRight = vecTargetToChara;
	vecRight.y = 0.f;
	vecRight.Cross({ 0,1,0 });
	vecRight.Normalize();
	ecPos += vecRight * charaSlideParam;

	//TODO : tarpがnanになるバグがあるので修正する. (m_targetPosは正常なのでこの演算中に壊れてる。
	//ターゲットのターゲット位置もカメラの右側にする.
	auto tarp = m_targetPos;
	tarp -= vecRight * targetSlideParam;
	tarp = m_charaPos + vecRight * charaSlideParam;
	//ターゲットの位置を上げる.
	tarp.y += cameraHeight;

	m_enemyCameraPos = ecPos;
	m_enemyCameraTargetPos = tarp;

	if (!m_isInitedInitialDist and m_state == State::enEnemyCamera) {
		m_oldDist = (m_enemyCameraTargetPos - m_enemyCameraPos).Length();
		m_isInitedInitialDist = true;
	}

}

void GameCamera::CalcPlayerCamera() {
	auto xf = g_pad[0]->GetRStickXF();
	auto yf = g_pad[0]->GetRStickYF();

	static const float upperLimit = 0.95f;
	static const float lowerLimit = -0.5f;
	static float cameraHeight = 5.f;

	auto vecCharaToCamera = m_playerCameraPos - m_charaPos;

	Quaternion rotY = Quaternion::Identity;
	Quaternion rotX = Quaternion::Identity;
	xf *= gameTime()->GetDeltaTime() * m_swivelSpeed;
	yf *= gameTime()->GetDeltaTime() * m_swivelSpeed;

	auto x = vecCharaToCamera;
	x.Normalize();

	auto vecRight = vecCharaToCamera;
	vecRight.y = 0.f;
	vecRight.Cross({ 0,1,0 });
	vecRight.Normalize();
	rotX.SetRotationDeg(vecRight, yf);
	rotX.Apply(m_dist);

	auto nd = m_dist;
	nd.Normalize();
	if (nd.y > upperLimit or nd.y < lowerLimit) {
		m_dist = m_old;
	}

	rotY.SetRotationDegY(xf);
	rotY.Apply(m_dist);

	m_playerCameraPos = m_charaPos + m_dist;
	m_playerCameraTargetPos = m_charaPos + m_furtherTargetHeight;

	//エネミーカメラに完全に切り替わったらプレイヤーカメラの位置を計算
	if (m_cameraChangeRatio == 0.f) {

		auto cameraForward = g_camera3D->GetForward();
		cameraForward.y = 0.f;
		cameraForward.Normalize();
		cameraForward.Scale(-1 * m_toCameraPos.Length());
		cameraForward.y += cameraHeight;
		m_playerCameraPos = m_charaPos + cameraForward;

		m_dist = m_playerCameraPos - GameManager::GetInstance().m_player->GetPosition();
	}

	m_old = m_dist;

	if (!m_isInitedInitialDist and m_state == State::enPlayerCamera) {
		m_oldDist = (m_playerCameraPos - m_playerCameraTargetPos).Length();
		m_isInitedInitialDist = true;
	}

}

void GameCamera::CalcSlerpPerformanceCamera() {

	//補完中
	auto vecCtoBCP = m_pfrmCamBeginPos - m_pfrmCenterPos;
	static float cameraDist = vecCtoBCP.Length();
	auto vecCtoECP = m_pfrmCamEndPos - m_pfrmCenterPos;

	float dist_cecp = vecCtoBCP.Length();
	float dist_cpcp = vecCtoECP.Length();

	vecCtoBCP.Normalize();
	vecCtoECP.Normalize();

	Quaternion q1;
	q1.SetRotation(vecCtoBCP, vecCtoECP);
	Quaternion sl;
	sl.Slerp(m_pfrmCameraChangeRatio, Quaternion::Identity, q1);
	Vector3 slpos = m_pfrmCamBeginPos - m_pfrmCenterPos;
	sl.Apply(slpos);
	auto scaleRatio = dist_cpcp / dist_cecp;
	auto scale = Math::Lerp(m_pfrmCameraChangeRatio, 1.f, scaleRatio);
	slpos.Scale(scale);

	m_position = slpos + m_pfrmCenterPos;

	g_camera3D->SetPosition(m_position);
	auto tar = Math::Lerp(m_pfrmCameraChangeRatio, m_pfrmTarBeginPos, m_pfrmTarEndPos);
	g_camera3D->SetTarget(tar);

	m_pfrmCameraChangeRatio = min(1.f, m_pfrmCameraChangeRatio += m_pfrmTimeSec * gameTime()->GetDeltaTime());

	if (m_pfrmCameraChangeRatio == 1.f) {
		m_state = State::enPlayerCamera;
		m_cameraChangeRatio = 1.f;
		GameManager::GetInstance().m_menu->ResumeGame();
	}

}

void GameCamera::CalcLerpPerformanceCamera() {

	Vector3 currentPos;
	currentPos.Lerp(m_pfrmCameraChangeRatio, m_pfrmCamBeginPos, m_pfrmCamEndPos);
	m_position = currentPos;
	g_camera3D->SetPosition(m_position);
	auto tar = Math::Lerp(m_pfrmCameraChangeRatio, m_pfrmTarBeginPos, m_pfrmTarEndPos);
	g_camera3D->SetTarget(tar);

	if (m_pfrmCameraChangeRatio == 1.f) {
		//m_state = State::enPlayerCamera;
		//m_cameraChangeRatio = 1.f;
		//GameManager::GetInstance().m_menu->ResumeGame();
		GameManager::GetInstance().m_gameScene->EndBossPerform();
	}

	if (m_pfrmTimer >= m_pfrmTimeSec + m_pfrmGameResumeInterval) {
		m_state = State::enPlayerCamera;
		GameManager::GetInstance().m_menu->ResumeGame();
		m_cameraChangeRatio = 1.f;
	}

	m_pfrmTimer = m_pfrmTimer + gameTime()->GetDeltaTime();
	m_pfrmCameraChangeRatio = min(1.f, m_pfrmTimer / m_pfrmTimeSec);
}

void GameCamera::UpdateState() {
	const auto targettingEnemy = EnemyManager::GetEnemyManager().GetTargettingEnemy();
	if (g_pad[0]->IsTrigger(enButtonRB3)) {
		if (m_state == State::enEnemyCamera) {
			m_state = State::enPlayerCamera;
		}
		else {
			m_state = State::enEnemyCamera;
		}
	}
}

std::tuple<int, int, int> GameCamera::GetTargetEnemyIndexes() {

	auto& enemies = EnemyManager::GetEnemyManager().GetEnemies();

	//カメラ正面の敵をターゲットする.
	const auto& cameraForward = g_camera3D->GetForward();
	auto cameraPos = g_camera3D->GetPosition();
	float maxApprox = -2.f;
	IEnemy* forwardEnemy = nullptr;
	float minDist = FLT_MAX;
	int forwardEnemyIndex = 0;

	const float AUTO_TARGET_RANGE = 300.f;

	std::map<float, int, std::greater<float>> sortedEnemiesMap;

	for (int i = 0; i < enemies.size(); i++) {
		const auto& enemyPos = enemies.at(i)->GetPosition();
		auto vecCameraToEnemy = enemyPos - cameraPos;
		if (vecCameraToEnemy.Length() < AUTO_TARGET_RANGE) {
			vecCameraToEnemy.Normalize();
			const auto approx = vecCameraToEnemy.Dot(cameraForward);
			//if (approx >= 0.5f)
			sortedEnemiesMap.insert(std::make_pair(approx, i));
		}
	}

	int index, iLeft, iRight;;
	index = iLeft = iRight = -1;

	//左右の最も正面に近い敵を調べる.
	if (sortedEnemiesMap.size() != 0) {

		for (auto const& e : sortedEnemiesMap) {
			//正面.
			if (index == -1) {
				index = e.second;
				continue;
			}

			//左右判定.
			auto vecCameraToEnemy = enemies.at(e.second)->GetPosition() - g_camera3D->GetPosition();
			vecCameraToEnemy.y = 0.f;
			vecCameraToEnemy.Normalize();
			Vector3 crs;
			crs.Cross(vecCameraToEnemy, cameraForward);

			//右.
			if (iRight == -1 and crs.y < 0.f) {
				iRight = e.second;
				continue;
			}
			//左.
			if (iLeft == -1 and crs.y > 0.f)
			{
				iLeft = e.second;
				continue;
			}
		}
	}

	return std::forward_as_tuple(index, iLeft, iRight);
}
//
//void GameCamera::Perform(
//	const Vector3& cameraBeginPos, const Vector3& cameraEndPos, const Vector3& targetBeginPos, const Vector3& targetEndPos, const Vector3& center, const float sec)
//{
//	GameManager::GetInstance().m_menu->PauseGame();
//	m_state = State::enSlerpPerformanceCamera;
//	m_pfrmCameraChangeRatio = 0.f;
//	m_pfrmTimeSec = sec;
//	m_pfrmCamBeginPos = cameraBeginPos;
//	m_pfrmCamEndPos = cameraEndPos;
//	m_pfrmTarBeginPos = targetBeginPos;
//	m_pfrmTarEndPos = targetEndPos;
//	m_pfrmCenterPos = center;
//}

void GameCamera::Perform(
	const Vector3& cameraBeginPos, const Vector3& cameraEndPos, const Vector3& targetBeginPos, const Vector3& targetEndPos, const float sec, const float resumeInterval)
{
	GameManager::GetInstance().m_menu->PauseGame();
	m_pfrmTimer = 0.f;
	m_state = State::enLerpPerformanceCamera;
	m_pfrmCameraChangeRatio = 0.f;
	m_pfrmTimeSec = sec;
	m_pfrmCamBeginPos = cameraBeginPos;
	m_pfrmCamEndPos = cameraEndPos;
	m_pfrmTarBeginPos = targetBeginPos;
	m_pfrmTarEndPos = targetEndPos;
	m_pfrmGameResumeInterval = resumeInterval;
}

