#include "stdafx.h"
#include "EnemyLongDistanceBlownState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Zako/Zako_LongDistanceMachine.h"
#include "Effect/ExplosionEffect.h"

EnemyLongDistanceBlownState::EnemyLongDistanceBlownState()
{
	m_effect = NewGO<ExplosionEffect>(0);
	m_effect->Init();
	m_effect->SetSca(Vector3::One * 0.2f);
}

EnemyLongDistanceBlownState::~EnemyLongDistanceBlownState()
{
	DeleteGO(m_effect);
}

void EnemyLongDistanceBlownState::Enter(IEnemy* e)
{
	auto ene = reinterpret_cast<Zako_LongDistanceMachine*>(e);
	m_body = &ene->GetRigidBody();
	ene->SetKinematicFrag(true);
	if (m_ik[0] == nullptr)
	{
		int i = 0;
		//m_ik[i++] = e->GetIK(TO_INT(IEnemy::EnIK::enChest));
		m_ik[i++] = e->GetIK(TO_INT(IEnemy::EnIK::enHead));
		m_ik[i++] = e->GetIK(TO_INT(Zako_LongDistanceMachine::EIK::Foot1));
		m_ik[i++] = e->GetIK(TO_INT(Zako_LongDistanceMachine::EIK::Foot2));
		m_ik[i++] = e->GetIK(TO_INT(Zako_LongDistanceMachine::EIK::Foot3));
		m_ik[i++] = e->GetIK(TO_INT(Zako_LongDistanceMachine::EIK::Foot4));
		for (auto ik : m_ik)
		{
			if (ik == nullptr)
				continue;
			ik->SetIKMode(IK::enMode_NoAnimHit);
		}
	}
	m_velocityXZ = e->GetKnockBackImpulse();
	m_velocityY = 0.f;
	m_isNoBlown = false;
	if (m_velocityXZ.y != 0.f)
		m_velocityY = max(m_velocityXZ.y / 10.f, 60.f);
	else
	{
		m_grav = 5.f;
		m_isNoBlown = true;
	}
	m_velocityXZ.y = 0.f;
	m_velocityXZ *= 0.1f;
	float xzLen = m_velocityXZ.Length();
	if (xzLen == 0.f)
	{
		m_velocityXZ = Vector3::Zero;
	}
	else if (xzLen < 60)
	{
		m_velocityXZ /= xzLen;
		m_velocityXZ *= 60.f;
	}
	m_timer = 0.f;
	m_isTakeOff = m_isNoBlown;
	m_isTakeOn = false;

	e->RemoveRigidBody();

	auto epos = e->GetPosition();
	
	auto b = m_body->GetBody();
	//b->getWorldTransform().setOrigin({ epos.x,epos.y+5.f ,epos.z });
	btTransform tra;
	ene->GetMotionState().getWorldTransform(tra);
	//tra.setOrigin({ epos.x,epos.y + 10.f ,epos.z });
	//ene->GetMotionState().setWorldTransform(tra);
	
	float poww = 1;
	//b->setAngularVelocity({ m_velocityXZ.x, m_velocityY , m_velocityXZ.z});
	
	
	//b->setAngularFactor(3);
	b->applyCentralImpulse({ m_velocityXZ.x * poww, m_velocityY * poww, m_velocityXZ.z * poww });
	//b->applyCentralForce({ m_velocityXZ.x * poww, m_velocityY * poww, m_velocityXZ.z * poww });
	b->setGravity({ 0,-160,0 });
	
	
	auto ppos = GameManager::GetInstance().GetPlayer()->GetPosition();
	//b->applyImpulse({ m_velocityXZ.x, m_velocityY, m_velocityXZ.z }, { ppos.x,ppos.y ,ppos.z });
	
	//b.
}

IEnemyState* EnemyLongDistanceBlownState::Update(IEnemy* e)
{
	if (m_timer >= m_timeLimit && !m_isPlayEffect)
	{
		m_effect->SetPos(e->GetPosition());
		m_effect->Play();
		m_isPlayEffect = true;
		//”š”­
		CSoundSource* se = NewGO<CSoundSource>(0, "Punch");
		se->Init(L"Assets/sound/mini_Explosion.wav");
		se->Play(false);
		se->SetVolume(0.1f);
	}
	
	float dtime = gameTime()->GetDeltaTime();

	auto ve = Vector3{ m_velocityXZ.x ,m_velocityY ,m_velocityXZ.z };

	
	{
		//m_ik[0]->SetVelocity(ve);
		//m_ik[1]->SetVelocity(ve);
		for (auto ik : m_ik)
		{
			if (ik == nullptr)
				continue;
			ik->SetVelocity(ve);
		}
	}


	auto b = m_body->GetBody();
	//DebugPrintVector3(EDebugConsoloUser::NOMOTO, b->getGravity());
	//b->applyDamping(dtime);
	//b->applyGravity();
	auto epos = e->GetPosition();
	m_effect->SetPos(epos);
	//b->applyImpulse({ m_velocityXZ.x, m_velocityY, m_velocityXZ.z }, { epos.x,epos.y ,epos.z });


	if (m_isTakeOff && e->IsOnGround())
	{
		m_isTakeOn = true;
		e->SetExternalVelocity(Vector3::Zero);
	}
	if (!m_isTakeOn || (/*!m_ikList[TO_INT(IEnemy::EnIK::enChest)]->IsHit() &&*/ m_isNoBlown))
		e->SetExternalVelocity(ve);


	if (!m_isTakeOff && !e->IsOnGround())
		m_isTakeOff = true;


	m_velocityY -= m_grav * dtime;
	m_timer += dtime;

	return this;
}

void EnemyLongDistanceBlownState::Exit(IEnemy* e)
{
}

