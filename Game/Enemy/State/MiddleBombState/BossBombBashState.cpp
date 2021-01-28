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

void BossBombBashState::Init(IEnemy* e)
{
	
	m_sphere.Create(15.0f);
	
	BossBombData::GetInstance().shieldGhost->Create(&m_sphere, e->GetPosition(), Quaternion::Identity);
}

void BossBombBashState::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(Boss_MiddleBomb::EnAnimEX::Bash));
	m_timer = 0.;
	//m_sphere.Create(1.0);
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

	Init(e);
	auto& BBData = BossBombData::GetInstance();
	for (int i = 0; i > BBData.shieldGhost->GetGhost()->getNumOverlappingObjects(); i++)
	{
		auto o = BBData.shieldGhost->GetGhost()->getOverlappingObject(i);
		int ind = o->getUserIndex();

		if (ind & GameCollisionAttribute::Player)
		{
			auto& p = GameManager::GetInstance().m_player;
			auto f = e->GetForward();
			p->ApplyDamage(10, true, f*200);
		}
	}
	//ボム敵データ取得
	

	//判定をとりだし
	ContactTestResult ctr;
	Physics().ContactTest(BBData.rigidBody->GetBody(), ctr);
	if (ctr.isHit && !m_isBashHit) {
		auto& p = GameManager::GetInstance().m_player;
		auto f = e->GetForward();
		f.Normalize();
		p->ApplyDamage(p->GetMaxHP()/15, true, f * 200);
		m_isBashHit = true;	//盾にめり込んで無限ハメ→死亡がありえるので追加

		CSoundSource* se = NewGO<CSoundSource>(0, "Bash");
		se->Init(L"Assets/sound/Bash.wav");
		se->Play(false);
		se->SetVolume(1.0f);
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
