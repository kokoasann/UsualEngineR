#include "stdafx.h"
#include "EnemyShortBigComing.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Zako/Zako_ShortBig.h"
#include "GameManager.h"

EnemyShortBigComing::EnemyShortBigComing()
{
}

EnemyShortBigComing::~EnemyShortBigComing()
{
}

void EnemyShortBigComing::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(IEnemy::EnAnimation::enRun));

	if (m_headIK == nullptr)
	{
		m_headIK = e->GetIK(TO_INT(IEnemy::EnIK::enHead));
	}
}

//�W�����v�A�^�b�N�֘A�ȊO��EnemyShortRangeComingState�Ɠ����B�v
IEnemyState* EnemyShortBigComing::Update(IEnemy* e)
{
	auto player = GameManager::GetInstance().m_player;
	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;

	//IK
	if (m_headIK != nullptr)
	{
		auto p = ppos;
		p.y += 10;
		m_headIK->SetNextTarget(p);
	}

	//�v���C���[���󒆂ɂ���Ȃ瓮���Ȃ��B
	const float airPlayerYPos = 70.0f;
	if (ppos.y > airPlayerYPos) {
		return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
	}

	const float moveRange = 10.f;
	const float attackRange = 10.f;
	const float idleRange = 150.f;

	const float vec2PlayerLen = vecToPlayer.Length();
	vecToPlayer /= vec2PlayerLen;

	if (player->GetCurrentHP() > 0.f) {

		float t = acosf(vecToPlayer.Dot(e->GetForward()));

		//�߂Â�����s���B
		if (vec2PlayerLen < attackRange && t < Math::PI * 0.3f) {
			auto rand = GRandom().Rand();
			//��30%�̊m���ōs���ω��B
			if (rand < 0.05f) {
				//�_���X�B
				return e->GetState(TO_INT(Zako_ShortBig::EnStateEX::enDance));
			}
			else if (rand < 0.7f) {
				//�p���`�B
				return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
			}
			else {
				//��U��̃p���`�B
				return e->GetState(TO_INT(IEnemy::EnState::enAttackB));
			}
		}
		//���ꂽ��ړ��B
		else {
			Move(e);
		}
		//���ꂷ������ړ���~�B
		if (vec2PlayerLen > idleRange) {
			return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
		}
	}

	//�����Ă���r���ł��܂ɗx��B
	const float maxTime = 2.5f;
	m_danceTimer += gameTime()->GetDeltaTime();
	if (m_danceTimer > maxTime) {
		auto rand = GRandom().Rand();
		//��5%�̊m���ŗx��B
		if (rand < 0.05f) {
			m_danceTimer = 0.f;
			//�_���X�B
			return e->GetState(TO_INT(Zako_ShortBig::EnStateEX::enDance));
		}
		m_danceTimer = 0.f;
	}

	return this;
}

void EnemyShortBigComing::Exit(IEnemy* e)
{
	e->SetVelocity(Vector3::Zero);
}

void EnemyShortBigComing::Move(IEnemy* e)
{
	auto player = GameManager::GetInstance().GetPlayer();

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	vecToPlayer.y = 0.f;
	vecToPlayer.Normalize();

	//���ʂ̃S�u�����̑��x��1/2�@�}�W�b�N�i���o�[�͕ύX�\��
	const float walkSpeed = 55.0f;
	Vector3 moveSpeed = vecToPlayer * walkSpeed;

	//�d�́B
	if (!e->IsOnGround())
		moveSpeed.y -= 100.0f;
	else
		DebugPrint_NOMOTO("is on ground");

	e->SetVelocity(moveSpeed);
}

bool EnemyShortBigComing::IsAnimation()
{
	return true;
}