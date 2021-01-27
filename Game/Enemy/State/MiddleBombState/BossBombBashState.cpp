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
struct ContactTestResult :public btCollisionWorld::ContactResultCallback
{
	bool isHit = false;

	// ContactResultCallback を介して継承されました
	virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
	{
		int ind = colObj0Wrap->getCollisionObject()->getUserIndex();
		//player かどうかの判定!
		if (ind & GameCollisionAttribute::Player)
			isHit = true;
		//	return 0.f;
		
		return 0.f;
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
	m_sphere.Create(1.0);
	m_isBashHit = false;
}

IEnemyState* BossBombBashState::Update(IEnemy* e)
{
	float dtime = gameTime()->GetDeltaTime();
	m_timer += gameTime()->GetDeltaTime();
	if (m_timer > m_timeLimit)
	{
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	//ボム敵データ取得
	auto& BBData = BossBombData::GetInstance();

	//判定をとりだし
	ContactTestResult ctr;
	Physics().ContactTest(BBData.rigidBody->GetBody(), ctr);
	if (ctr.isHit && !m_isBashHit) {
		auto& p = GameManager::GetInstance().m_player;
		auto f = e->GetForward();
		f.Normalize();
		p->ApplyDamage(10, true, f * 100);
		m_isBashHit = true;	//盾にめり込んで無限ハメ→死亡がありえるので追加
	}

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
