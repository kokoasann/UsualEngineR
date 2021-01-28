#include "stdafx.h"
#include "Boss_FatmanTakeDistanceState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_Fatman.h"

Boss_FatmanTakeDistanceState::Boss_FatmanTakeDistanceState()
{
	m_sphereCollider.Create(m_radius);
}

Boss_FatmanTakeDistanceState::~Boss_FatmanTakeDistanceState()
{
}

void Boss_FatmanTakeDistanceState::Enter(IEnemy* e)
{
	const auto& rot = Boss_Fatman::EnemyToPlayerRotation(e, false);
	e->GetModel()->SetRotation(rot);
}

struct WallFromBossCallBack : public btCollisionWorld::ConvexResultCallback
{
	//障害物があるかないか判定。
	bool isHit = false;
	Vector3 wallNormalVec;
	Vector3 startPos;
	float dist = FLT_MAX;

	//衝突したら勝手に呼んでくれる。
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		int index = convexResult.m_hitCollisionObject->getUserIndex();

		//上方向と法線のなす角度を求める。
		Vector3 hitNormalTmp = convexResult.m_hitNormalLocal;
		float angle = hitNormalTmp.Dot(g_vec3Up);
		angle = acosf(angle);

		if (angle > Math::PI * 0.1f	//地面の傾斜が54度より小さいので地面とみなす。
			&& index & enCollisionAttr_Ground){
			Vector3 hitpos = convexResult.m_hitPointLocal;
			Vector3 hp = startPos - hitpos;
			float disttmp = hp.Length();
			if (dist > disttmp) {
				dist = disttmp;
				//当たった。
				isHit = true;
				wallNormalVec = convexResult.m_hitNormalLocal;
				wallNormalVec.y = 0.0f;
				wallNormalVec.Normalize();
			}			
		}
		return 0.0f;
	}

};


struct WallCallBack : public btCollisionWorld::ConvexResultCallback
{
	//障害物があるかないか判定。
	bool isHit = false;
	Vector3 startPos;
	Vector3 wallNormalVec;
	float dist = FLT_MAX;

	//衝突したら勝手に呼んでくれる。
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		int index = convexResult.m_hitCollisionObject->getUserIndex();

		//上方向と法線のなす角度を求める。
		Vector3 hitNormalTmp = convexResult.m_hitNormalLocal;
		float angle = hitNormalTmp.Dot(g_vec3Up);
		angle = acosf(angle);

		if (angle > Math::PI * 0.33f	//地面の傾斜が54度より小さいので地面とみなす。
			&& index & enCollisionAttr_Ground){
			Vector3 hitpos = convexResult.m_hitPointLocal;
			Vector3 hp = startPos - hitpos;
			float disttmp = hp.Length();
			if (dist > disttmp) {
				dist = disttmp;
				//当たった。
				isHit = true;
				wallNormalVec = convexResult.m_hitNormalLocal;
				wallNormalVec.y = 0.0f;
				wallNormalVec.Normalize();
			}			
		}
		return 0.0f;
	}

};

IEnemyState* Boss_FatmanTakeDistanceState::Update(IEnemy* e)
{
	
	auto& p = GameManager::GetInstance().m_player;
	const auto& epos = e->GetPosition();
	const auto& ppos = p->GetPosition();
	auto vecToBoss = epos - ppos;
	float distance = vecToBoss.Length();

	if (std::abs(distance) > 100.f){
		e->SetVelocity(Vector3::Zero);
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}
	vecToBoss.Normalize();
	const float applyMovespeed = 20.0f;
	Vector3 movespeed = vecToBoss * applyMovespeed;
	//コリジョンの移動の始点と終点の設定。
	btTransform start, end;
	Vector3 startPos;
	{
		//回転の設定。
		start.setIdentity();
		end.setIdentity();
		//座標の設定。
		startPos = epos;
		startPos.y += 5.0f;
		start.setOrigin(btVector3(startPos.x, startPos.y, startPos.z));
		Vector3 endPos = startPos + movespeed * gameTime()->GetDeltaTime();
		end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
	}
	WallFromBossCallBack cb;
	cb.startPos = startPos;
	//startからendまでコリジョンを移動させて当たり判定を取る。
	Physics().ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, cb, 0.f);
	//重力。
	Vector3 gravity = Vector3::Zero;
	gravity.y = -5000.f;
	if (cb.isHit) {
		Vector3 vec;
		vec.Cross(Vector3::AxisY, cb.wallNormalVec);
		vec.y = 0.0f;
		vec.Normalize();
		vec *= applyMovespeed;

		Vector3 ENewPos = vec + epos;
		Vector3 distvec = ENewPos - ppos;

		Vector3 ENewPos2 = vec * -1 + epos;
		Vector3 distvec2 = ENewPos2 - ppos;

		if (distvec.Length() > distvec2.Length()) {
			vec *= -1;
		}

		vec += gravity;
		e->SetVelocity(vec);
	}
	else {
		//コリジョンの移動の始点と終点の設定。
		btTransform start, end;
		Vector3 startPos;
		{
			//回転の設定。
			start.setIdentity();
			end.setIdentity();
			//座標の設定。
			startPos = epos;
			startPos.y += 5.0f;
			start.setOrigin(btVector3(startPos.x, startPos.y, startPos.z));
			Vector3 moveVec = Vector3::AxisY * -1;
			Vector3 endPos = startPos + moveVec * 20.0f;
			end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
		}
		WallCallBack cb;
		cb.startPos = startPos;
		//startからendまでコリジョンを移動させて当たり判定を取る。
		Physics().ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, cb, 0.f);

		if (cb.isHit) {
			movespeed *= -1;
			Vector3 vec;
			vec.Cross(Vector3::AxisY, cb.wallNormalVec);
			vec.y = 0.0f;
			vec.Normalize();
			vec *= applyMovespeed;

			Vector3 ENewPos = vec + epos;
			Vector3 distvec = ENewPos - ppos;

			Vector3 ENewPos2 = vec * -1 + epos;
			Vector3 distvec2 = ENewPos2 - ppos;

			if (distvec.Length() > distvec2.Length()) {
				vec *= -1;
			}

			vec += gravity;
			e->SetVelocity(vec);
			return this;
		}

		movespeed += gravity;
		e->SetVelocity(movespeed);
	}	
	return this;
}

void Boss_FatmanTakeDistanceState::Exit(IEnemy* e)
{
}
