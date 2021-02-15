#pragma once
#include "../Attack/PlayerLongRangeAttack.h"

class Player;
class JetEffect;
class ExplosionEffect;
class Beam;
class PlayerBulletManager;

/// <summary>
/// 
/// </summary>
class Pod :public GameObject
{
public:
	Pod();
	virtual ~Pod();

	struct PodAbility {
		const float STAMINA_MAX = 100.f;
		float currentStamina = STAMINA_MAX;
		const float StaminaChargeSpeed = 5.f;
	};


	/// <summary>
	/// 本開放。確保したものを開放するための関数。
	/// </summary>
	virtual void Release() override;

	/// <summary>
	/// このオブジェクトの所持するゲームオブジェクト等を消すための関数。
	/// 面倒くさければRelease関数と同じでもよい。
	/// </summary>
	virtual void OnDestroy() override;

	/// <summary>
	/// NewGO時に即座に呼ばれる関数。
	/// コンストラクタの代わりに使おう。
	/// </summary>
	void Awake() override;

	/// <summary>
	/// スタート関数。
	/// </summary>
	/// <returns>trueを返さない限り、ずっとStartを呼び続ける</returns>
	bool Start() override;

	/// <summary>
	/// 更新。の前に呼ばれる更新。
	/// </summary>
	void PreUpdate() override;
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
	/// <summary>
	/// 更新。の後に呼ばれる更新。
	/// </summary>
	void PostUpdate() override;

	/// <summary>
	/// 描画。
	/// </summary>
	void Render() override;

	/// <summary>
	/// 手前に表示するやつの描画。
	/// </summary>
	void PostRender() override;

	void SetPosition(const Vector3& pos) {
		m_pos = pos;
		m_model->SetPosition(m_pos);
	}

	void SetRotation(const Quaternion& rot) {
		m_rotation = rot;
		m_model->SetRotation(rot);
	}

	const Vector3& GetPosition() const {
		return m_pos;
	}

	/// <summary>
	/// Podを投げる
	/// </summary>
	/// <param name="velocity">Velocity</param>
	/// <param name="thrownTime">何秒間投げるか</param>
	void Thrown(const Vector3& velocity, const float thrownTime = 1.f) {
		m_velocity = velocity;
		m_thrownTime = thrownTime;
		m_state = PodState::enThrown;
		m_timer = 0.f;
	}

	const bool IsIdle() {
		return m_state == PodState::enIdle;
	}

	enum class EnPodBone {
		Burrel_L1,
		Burrel_L2,
		Burrel_R1,
		Burrel_R2,
		Thruster_Back,
		Thruster_Under,
		enNumBoneType
	};

	Bone* GetBone(Pod::EnPodBone bone) {
		return m_podBones.at(TO_INT(bone));
	}

	std::vector<Bone*>& GetBones() {
		return m_podBones;
	}

	const Quaternion& GetRotation() {
		return m_rotation;
	}

private:
	bool IsHitBombShield(const Vector3& vel);

	void ShotLaserBeam();
	void ThrownBehave();
	void Rampage();
	void Kamikaze();
	void BackToIdlePos();

	void IdleRotation();
	void CalcIdlePosition();

	void UseStamina(const float amount) {
		m_ability.currentStamina = max(0.f, m_ability.currentStamina - amount);
	}

	void ChargeStamina(const float amount) {
		m_ability.currentStamina = min(m_ability.STAMINA_MAX, m_ability.currentStamina + amount);
	}

	//model
	ModelRender* m_model = nullptr;

	//transform
	Vector3 m_pos = Vector3::Zero;
	const Vector3 m_distanceFromPlayer = { 5.f,10.f,-5.f };
	Quaternion m_rotation = Quaternion::Identity;
	const Vector3 m_scale = { 1.5f,1.5f,1.5f };
	float m_timer = 0.f;

	Vector3 m_velocity = Vector3::Zero;

	//laser
	const float m_LaserDamageAmount = 10.f;

	//Thrown
	float m_thrownTime = 1.f;
	const float m_thrownAttackDamageAmount = 20.f;
	const float m_thrownAttackRange = 30.f;

	//Rampage
	bool m_isIncreasing = true;
	float m_repetitiveMovementParam = 0.5f;
	bool m_isChasingEnemy = false;
	const float m_rampageTime = 5.f;
	const float m_rampagingDamageAmount = 10.f;

	//Kamikaze
	const float m_kamikazeDamageAmount = 10.f;

	//Back
	const float m_backSpeed = 150.f;

	//State
	enum class PodState {
		enIdle,
		enThrown,
		enRampage,
		enKamikaze,
		enBack,
		enNumPodState
	};

	PodState m_state = PodState::enIdle;

	PlayerLongRangeAttack m_longRangeAttack;

	//Effects

	const int BACK = 0;
	const int UNDER = 1;

	std::vector<Bone*> m_podBones;
	std::vector<JetEffect*> m_jetEffects;
	//SmokeEffect* m_smokeEffect;

	struct SkillCosts {
		const float ProjectileCost = 100.f / 5.f;
		const float LaserCost = 30.f;
		const float RampageCost = 30.f;
		const float KamikazeCost = 30.f;
	} skillCosts;

	PodAbility m_ability;

	bool m_overheat = false;
	float m_overheatTimer = 0.f;
	const float m_OverheatTime = 3.f;

	typedef Vector4 Color;
	Color m_mulCol = Color::White;
	//Color m_smokeCol = { 0.3,0.3,0.3,0.5f };
	ExplosionEffect* m_explosionEffect = nullptr;
	Beam* m_beamEffect = nullptr;


	//CSoundSource* m_laserSE = nullptr;
	CSoundSource* m_gunSE = nullptr;

	SphereCollider m_sphere;

	PlayerBulletManager* m_playerBulletManager;
};
