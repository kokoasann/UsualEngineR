#pragma once

#include "Enemy/IEnemy.h"
#include "Effect/ExplosionEffect.h"
/// <summary>
/// 
/// </summary>
class Boss_MiddleBomb final :public IEnemy
{
public:
	Boss_MiddleBomb();
	virtual ~Boss_MiddleBomb();

	void Init() override;
	void InitState() override;
	void InitIK() override;
	void Execute() override;
	void Terminate() override;
	Player::EnAttackType GetDropID() override
	{
		return Player::EnAttackType::enNone;
	}

	void PlayAnimation(const int animNo, const float interpolate = 0.2f)
	{
		m_model->Play(animNo, interpolate);
	}
	
	enum class EnStateEX :int
	{
		Luanch = TO_INT(IEnemy::EnState::enAttackA),
		Corocoro,
		FullFrontal = TO_INT(IEnemy::EnState::enNumState),
		Bash,
		Rolling,
		Guard,
		Jump,
		Num,
	};

	enum class EnAnimEX :int
	{
		Firing = TO_INT(IEnemy::EnAnimation::enAttackA),
		FiringUp,
		FullFrontal = TO_INT(IEnemy::EnAnimation::enNumAnim),
		Bash,
		Guard,
		Jump,
		Angry,
		Num,
	};

	void Explode() override {
		auto forward = GetForward();
		forward *= -30.f;
		m_explodeEffect->SetPos(m_position + forward);
		//m_explodeEffect->SetPos(Vector3::One);
		m_explodeEffect->Play();
		StopJetSE();
		auto explodeSe = NewGO<CSoundSource>(0);
		explodeSe->Init(L"Assets/sound/chara/explosion.wav", true);
		explodeSe->SetPosition(m_position);
		explodeSe->Play(false);
	}
private:
	void InitAnim();
private:
	//Model
	ModelRender* m_ShieldModel = nullptr;
	const float m_scale = 1.f;

	RigidBody m_rigidBody;
	MeshCollider m_meshColl;
	MeshCollider m_meshColl_ghost;
	Bone* m_shieldBone = nullptr;

	GhostObject m_shieldGhost;

	//Physics
	const float m_radius = 1.f;
	const float m_height = 7.f;
};

struct BossBombData
{
private:
	BossBombData() {};
public:

	enum class EnFeel
	{
		Normale,
		Angry,
		Tired
	};
	static BossBombData& GetInstance()
	{
		static BossBombData data;
		return data;
	}
	EnFeel feeling = EnFeel::Normale;

	RigidBody* rigidBody = nullptr;
	MeshCollider* meshColl = nullptr;
	GhostObject* shieldGhost = nullptr;
	bool isJumpStart = false;
	bool isBashAttack = false;
	bool isStartBattle = false;
};
