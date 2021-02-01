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
	//��Q�������邩�Ȃ�������B
	bool isHit = false;
	Vector3 wallNormalVec;
	Vector3 startPos;
	float dist = FLT_MAX;

	//�Փ˂����珟��ɌĂ�ł����B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		int index = convexResult.m_hitCollisionObject->getUserIndex();

		//������Ɩ@���̂Ȃ��p�x�����߂�B
		Vector3 hitNormalTmp = convexResult.m_hitNormalLocal;
		float angle = hitNormalTmp.Dot(g_vec3Up);
		angle = acosf(angle);

		if (angle > Math::PI * 0.1f	//�n�ʂ̌X�΂�54�x��菬�����̂Œn�ʂƂ݂Ȃ��B
			&& index & enCollisionAttr_Ground){
			Vector3 hitpos = convexResult.m_hitPointLocal;
			Vector3 hp = startPos - hitpos;
			float disttmp = hp.Length();
			if (dist > disttmp) {
				dist = disttmp;
				//���������B
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
	//��Q�������邩�Ȃ�������B
	bool isHit = false;
	Vector3 startPos;
	Vector3 wallNormalVec;
	float dist = FLT_MAX;

	//�Փ˂����珟��ɌĂ�ł����B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		int index = convexResult.m_hitCollisionObject->getUserIndex();

		//������Ɩ@���̂Ȃ��p�x�����߂�B
		Vector3 hitNormalTmp = convexResult.m_hitNormalLocal;
		float angle = hitNormalTmp.Dot(g_vec3Up);
		angle = acosf(angle);

		if (angle > Math::PI * 0.33f	//�n�ʂ̌X�΂�54�x��菬�����̂Œn�ʂƂ݂Ȃ��B
			&& index & enCollisionAttr_Ground){
			Vector3 hitpos = convexResult.m_hitPointLocal;
			Vector3 hp = startPos - hitpos;
			float disttmp = hp.Length();
			if (dist > disttmp) {
				dist = disttmp;
				//���������B
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
	//�R���W�����̈ړ��̎n�_�ƏI�_�̐ݒ�B
	btTransform start, end;
	Vector3 startPos;
	{
		//��]�̐ݒ�B
		start.setIdentity();
		end.setIdentity();
		//���W�̐ݒ�B
		startPos = epos;
		startPos.y += 5.0f;
		start.setOrigin(btVector3(startPos.x, startPos.y, startPos.z));
		Vector3 endPos = startPos + movespeed * gameTime()->GetDeltaTime();
		end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
	}
	WallFromBossCallBack cb;
	cb.startPos = startPos;
	//start����end�܂ŃR���W�������ړ������ē����蔻������B
	Physics().ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, cb, 0.f);
	//�d�́B
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
		//�R���W�����̈ړ��̎n�_�ƏI�_�̐ݒ�B
		btTransform start, end;
		Vector3 startPos;
		{
			//��]�̐ݒ�B
			start.setIdentity();
			end.setIdentity();
			//���W�̐ݒ�B
			startPos = epos;
			startPos.y += 5.0f;
			start.setOrigin(btVector3(startPos.x, startPos.y, startPos.z));
			Vector3 moveVec = Vector3::AxisY * -1;
			Vector3 endPos = startPos + moveVec * 20.0f;
			end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
		}
		WallCallBack cb;
		cb.startPos = startPos;
		//start����end�܂ŃR���W�������ړ������ē����蔻������B
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
