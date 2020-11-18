#pragma once
#include "../IEnemy.h"
#include "../../Player/Player.h"

class JetEffect;

/// <summary>
/// 
/// </summary>
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

	enum class EnState {
		enIdleState,
		enBattleState,
		enFlyState,
		enTeleportation,
		enPunch,
		enDashPunch,
		enOverheat,
		enDeadState,
		enNumState
	};

private:

	//Model
	const Vector3 m_scale = { 1.f,1.f,1.f };
	CAnimationClip m_animClip[1];
	std::vector<CAnimationClipPtr> m_animlist;

	//Effect
	enum class EnJetBone {
		ElbowR,
		ElbowL,
		ThrusterR,
		ThrusterL,
		BackR,
		BackL,
		Skirt,
		NumJetBone
	};

	std::vector<Bone*> m_bones;
	std::vector<JetEffect*> m_jetEffects;

	//Physics
	const float m_radius = 1.f;
	const float m_height = 3.f;
};
