#include "stdafx.h"
#include "GameCamera.h"
#include "../Player/Player.h"
#include "../Enemy/IEnemy.h"
#include "../PositionChecker.h"
#include "Enemy/EnemyManager.h"
#include <limits>

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

	g_camera3D->SetTarget(m_targetPos);
	m_playerCameraPreviousPos = m_playerCameraPos = m_position = m_charaPos + m_position;

	g_camera3D->SetTarget({ 0.0f, 0.0f, 0.0f });
	g_camera3D->SetPosition(m_position);

	//m_toCameraPos.Set(0.0f, 3.0f, -15.f);
	m_toCameraPos.Set(0.0f, 6.0f, -30.f);
	m_dist = m_toCameraPos;

	m_cameraCollisionSolver.Init(m_sphereCollisionRadius);

	auto& enemies = EnemyManager::GetEnemyManager().GetEnemies();
	if (enemies.size() == 0) {
		mp_enemy = nullptr;
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
	Vector3 target = mp_player->GetPosition();
	m_charaPos = mp_player->GetPosition();
	auto& enemies = EnemyManager::GetEnemyManager().GetEnemies();

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

	if (enemies.size() == 0) {
		mp_enemy = nullptr;
	}
	else {
		//�^�[�Q�b�g�ؑ�.
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
				else if(centerIndex != -1){
					m_targetEnemyNo = centerIndex;
				}
			}

			//left
			if (rxf <= -changeTargetVal) {
				isChangeTarget = true;
				std::tie(centerIndex, leftIndex, rightIndex) = GetTargetEnemyIndexes();
				if (rightIndex != -1) {
					m_targetEnemyNo = leftIndex;
				}
				else if (centerIndex != -1) {
					m_targetEnemyNo = centerIndex;
				}
			}

			if (m_targetEnemyNo != -1 and isChangeTarget) {
				enemyTargetChangeTime = 0.f;
				m_enemyCameraNextTargetPos = enemies.at(m_targetEnemyNo)->GetPosition();
				mp_enemy = enemies.at(m_targetEnemyNo);
				mp_player->SetTargetEnemy(mp_enemy);
			}
		}
	}

	UpdateState();
	CalcEnemyCamera();
	CalcPlayerCamera();
}

void GameCamera::PostUpdate() {

	const float PlayerCameraRatio = 1.f;
	const float EnemyCameraRatio = 0.f;

	if (m_cameraChangeRatio == PlayerCameraRatio) {
		m_position = m_playerCameraPos;
	}
	else if (m_cameraChangeRatio == EnemyCameraRatio) {
		m_position = m_enemyCameraPos;
	}
	else {
		//�⊮��
		auto vecCtoECP = m_enemyCameraPos - mp_player->GetPosition();
		static float cameraDist = vecCtoECP.Length();
		auto vecCtoPCP = m_playerCameraPos - mp_player->GetPosition();

		float dist_cecp = vecCtoECP.Length();
		float dist_cpcp = vecCtoPCP.Length();

		vecCtoECP.Normalize();
		vecCtoPCP.Normalize();

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
	}

	g_camera3D->SetPosition(m_position);
	auto tar = Math::Lerp(m_cameraChangeRatio, m_enemyCameraTargetPos, m_playerCameraTargetPos);
	g_camera3D->SetTarget(tar);

	//�J���������蔻��.
	Vector3 result;
	auto flag = m_cameraCollisionSolver.Execute(
		result,
		g_camera3D->GetPosition(),
		g_camera3D->GetTarget()
	);

	//�}�ɋ������ς���Ă��Ȃ������ׂ�.
	float dlt = 0.f;
	float dist = (result - tar).Length();
	dlt = m_oldDist - dist;
	//�J�������r�Ԃ�悤�ȃ��U���g�̏ꍇ�͖���.
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

void GameCamera::CalcEnemyCamera() {
	static float distParam = 18.f;
	static float cameraHeight = 8.f;
	static float charaSlideParam = 5.f;
	static float targetSlideParam = 30.f;

	Vector3 ecPos;

	if (mp_enemy != nullptr) {
		m_targetPos = mp_enemy->GetPosition();
		m_enemyCameraNextTargetPos = mp_enemy->GetPosition();
	}

	//�^�[�Q�b�g�̈ʒu��ؑ֐�܂ŕ⊮����.
	auto vecCurrentToNext = m_enemyCameraNextTargetPos - m_enemyCameraCurrentTargetPos;
	auto lengthToNextTargetPos = vecCurrentToNext.Length();
	vecCurrentToNext.Normalize();
	auto currentLength = lengthToNextTargetPos * enemyTargetChangeTime;
	m_enemyCameraCurrentTargetPos += vecCurrentToNext * currentLength;
	enemyTargetChangeTime += gameTime()->GetDeltaTime();
	enemyTargetChangeTime = min(gameTime()->GetDeltaTime() + enemyTargetChangeTime, 1.f);
	m_targetPos = m_enemyCameraCurrentTargetPos;

	//�L�����ƃ^�[�Q�b�g���꒼���ɏd�Ȃ��Č�����悤�ȃJ�����̈ʒu���v�Z.
	auto vecTargetToChara = m_charaPos - m_targetPos;
	auto length = vecTargetToChara.Length();
	vecTargetToChara.Normalize();
	ecPos = m_targetPos + vecTargetToChara * (length + distParam);
	//�J�����̈ʒu���グ��.
	ecPos.y += cameraHeight;

	//�J�������E���ɂ��炷.
	auto vecRight = vecTargetToChara;
	vecRight.y = 0.f;
	vecRight.Cross({ 0,1,0 });
	vecRight.Normalize();
	ecPos += vecRight * charaSlideParam;

	//TODO : tarp��nan�ɂȂ�o�O������̂ŏC������. (m_targetPos�͐���Ȃ̂ł��̉��Z���ɉ��Ă�B
	//�^�[�Q�b�g�̃^�[�Q�b�g�ʒu���J�����̉E���ɂ���.
	auto tarp = m_targetPos;
	tarp -= vecRight * targetSlideParam;
	tarp = m_charaPos + vecRight * charaSlideParam;
	//�^�[�Q�b�g�̈ʒu���グ��.
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

	//�G�l�~�[�J�����Ɋ��S�ɐ؂�ւ������v���C���[�J�����̈ʒu���v�Z
	if (m_cameraChangeRatio == 0.f) {

		auto cameraForward = g_camera3D->GetForward();
		cameraForward.y = 0.f;
		cameraForward.Normalize();
		cameraForward.Scale(-1 * m_toCameraPos.Length());
		cameraForward.y += cameraHeight;
		m_playerCameraPos = m_charaPos + cameraForward;

		m_dist = m_playerCameraPos - mp_player->GetPosition();
	}

	m_old = m_dist;

	if (!m_isInitedInitialDist and m_state == State::enPlayerCamera) {
		m_oldDist = (m_playerCameraPos - m_playerCameraTargetPos).Length();
		m_isInitedInitialDist = true;
	}

}

void GameCamera::UpdateState() {
	if (g_pad[0]->IsTrigger(enButtonRB3)) {
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

std::tuple<int, int, int> GameCamera::GetTargetEnemyIndexes() {

	auto& enemies = EnemyManager::GetEnemyManager().GetEnemies();

	//�J�������ʂ̓G���^�[�Q�b�g����.
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

	//���E�̍ł����ʂɋ߂��G�𒲂ׂ�.
	if (sortedEnemiesMap.size() != 0) {

		for (auto const& e : sortedEnemiesMap) {
			//����.
			if (index == -1) {
				index = e.second;
				continue;
			}

			//���E����.
			auto vecCameraToEnemy = enemies.at(e.second)->GetPosition() - g_camera3D->GetPosition();
			vecCameraToEnemy.y = 0.f;
			vecCameraToEnemy.Normalize();
			Vector3 crs;
			crs.Cross(vecCameraToEnemy, cameraForward);

			//�E.
			if (iRight == -1 and crs.y < 0.f) {
				iRight = e.second;
				continue;
			}
			//��.
			if (iLeft == -1 and crs.y > 0.f)
			{
				iLeft = e.second;
				continue;
			}
		}
	}

	return std::forward_as_tuple(index, iLeft, iRight);
}
