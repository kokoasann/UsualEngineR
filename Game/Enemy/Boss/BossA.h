#pragma once
#include "../IEnemy.h"
#include "../../Player/Player.h"
#include "physics/SphereCollider.h"

class JetEffect;

class BossA :public IEnemy
{
public:
	void Init() override;
	void InitState() override;
	void Execute() override;
	void Terminate() override;

	Player::EnAttackType GetDropID() override{
		return Player::EnAttackType::enRemoteAttackPreset;
	}

	void PlayAnimation(const EnAnimation& anim, const float interpolate = 0.2f) override{
		m_model->Play(static_cast<int>(anim), interpolate);
	}

	void PlayAnimation(const int animNo, const float interpolate = 0.2f) override{
		m_model->Play(animNo, interpolate);
	}

	enum class EnState {
		enFlyState = TO_INT(IEnemy::EnState::enNumState),
		enTeleportation,
		enPunch,
		enDashPunch,
		enOverheat,
		enNumState
	};

	//Effect
	enum class EnJetBone {
		ElbowR,
		ElbowL,
		ThrusterR,
		ThrusterL,
		BackR,
		BackL,
		Skirt,
		//HandR,
		//HandL,
		NumJetBone
	};

	void Performance()
	{
		if (GetCurrentState() == GetState(TO_INT(IEnemy::EnState::enDeadState))) {
			m_model->SetMulColor({ 0.f,0.f,0.f,1.f });
		}
		else {
			m_model->SetMulColor({ 20,1,1,1 });
		}
	}
private:

	//Model
	const Vector3 m_scale = { 1.f,1.f,1.f };
	CAnimationClip m_animClip[1];

	//Physics
	const float m_radius = 5.5f;
	const float m_height = 3.f;
	//Hands
	//SphereCollider		m_rightHandCollider;
	//const float m_HandRadius = 1.f;
};
