#include "stdafx.h"
#include "Enemy_Bullet.h"

#include "GameManager.h"
#include "Player/Player.h"


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
		Vector3 hitp = *(Vector3*)&convexResult.m_hitPointLocal;
		Vector3 div = startPos - hitp;
		float dis = div.Length();
		if (dis < dist)
		{
			hitPos = *(Vector3*)&convexResult.m_hitPointLocal;
			dist = dis;
			collAttr = index;
		}
		return 0.0f;
	}
};


Enemy_Bullet::Enemy_Bullet()
{

}

Enemy_Bullet::~Enemy_Bullet()
{

}



void Enemy_Bullet::Release()
{

}

void Enemy_Bullet::OnDestroy()
{
	Release();
}

void Enemy_Bullet::Init(const Vector3& pos, float scale, const Vector3& dir, float speed, float timeLimit, float attackRenge)
{
	m_dir = dir;
	m_speed = speed;
	m_scale = scale;
	m_timeLimit = timeLimit;
	m_attackRange = attackRenge;

	Quaternion rot;
	rot.SetRotation(Vector3::AxisZ * -1.f, m_dir);

	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test_criss.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(Vector3::One * m_scale);
	m_model->SetPosition(pos);
	m_model->SetRotation(rot);
	
}


void Enemy_Bullet::Awake()
{

}

bool Enemy_Bullet::Start()
{
	return true;
}


void Enemy_Bullet::PreUpdate()
{

}

void Enemy_Bullet::Update()
{
	float dtime = gameTime()->GetDeltaTime();

	Vector3 oldpos = m_model->GetPosition();
	Vector3 newpos = oldpos + m_dir * (m_speed * m_timer);

	btTransform start, end;
	start.setOrigin({ oldpos.x, oldpos.y, oldpos.z });
	end.setOrigin({ newpos.x, newpos.y, newpos.z });
	SweepResult sr(oldpos);
	Physics().ConvexSweepTest(m_sphere.GetBody(), start, end, sr);
	
	if (sr.collAttr & enCollisionAttr_Character)
	{
		auto& gm =  GameManager::GetInstance();

		Vector3 b2p = gm.m_player->GetPosition() - sr.hitPos;
		float b2pLen = b2p.Length();

		if (m_attackRange > b2pLen)
		{

		}

		auto pthis = this;
		DeleteGO(pthis);
	}
	else
	{
		auto pthis = this;
		DeleteGO(pthis);
	}

	m_model->SetPosition(newpos);
}

void Enemy_Bullet::PostUpdate()
{

}



