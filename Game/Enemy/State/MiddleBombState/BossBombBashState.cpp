#include "stdafx.h"
#include "BossBombBashState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "GameManager.h"

#include "Enemy/Equipment/Enemy_LaunchBomb.h"


struct SweepResult : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;
	Vector3 hitPos = Vector3::Zero;
	const Vector3& startPos;
	float dist = FLT_MAX;
	int collAttr = enCollisionAttr_None;

	SweepResult(const Vector3& v) :
		startPos(v)
	{

	}

	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		int index = convexResult.m_hitCollisionObject->getUserIndex();
		//キャラコンでもグラウンドでもない場合早期出社。
		if (!(index & enCollisionAttr_Character
			|| index & enCollisionAttr_Ground))
		{
			return 0.f;
		}

		isHit = true;
		Vector3 hitp = *(Vector3*)& convexResult.m_hitPointLocal;
		Vector3 div = startPos - hitp;
		float dis = div.Length();
		if (dis < dist)
		{
			hitPos = *(Vector3*)& convexResult.m_hitPointLocal;
			dist = dis;
			collAttr = index;
		}
		return 0.0f;
	}
};
BossBombBashState::BossBombBashState()
{
}

BossBombBashState::~BossBombBashState()
{
}

void BossBombBashState::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(Boss_MiddleBomb::EnAnimEX::Bash));
	m_timer = 0.;
	m_sphere.Create(0.5);
}

IEnemyState* BossBombBashState::Update(IEnemy* e)
{
	float dtime = gameTime()->GetDeltaTime();
	m_timer += gameTime()->GetDeltaTime();
	if (m_timer > m_timeLimit)
	{
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	//バッシュ判定を作るために情報を入れる
	//RigidBodyInfo info;
	//info.pos = e->GetPosition();
	//info.rot = e->GetRotation();
	//info.sca = g_vec3One;
	m_dir = e->GetPosition();
	m_dir.Normalize();
	//ボム敵データ取得
	auto BBData = BossBombData::GetInstance();
	Vector3 oldpos = e->GetModel()->GetPosition();
	Vector3 newpos = oldpos + m_dir * (m_speed * dtime);
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	start.setOrigin({ oldpos.x, oldpos.y, oldpos.z });
	end.setOrigin({ newpos.x, newpos.y, newpos.z });
	SweepResult sr(oldpos);
	Physics().ConvexSweepTest(m_sphere.GetBody(),start, end, sr);
	//判定をとりだし
	//Physics().ContactTest(BBData.rigidBody->GetBody(),sr);

	//m_rigidBody.Create(info);
	//m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Wall | GameCollisionAttribute::BombShield);
	//Physics().AddRigidBody(m_rigidBody);

	return this;
}

void BossBombBashState::Exit(IEnemy* e)
{
}

void BossBombBashState::OnAttacked(IEnemy* e)
{
}
