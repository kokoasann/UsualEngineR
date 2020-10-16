#pragma once
#include "State/IPlayerSstate.h"
#include "../../UsualEngineR/Character/CharacterController.h"
class IEnemy;

/// <summary>
/// 
/// </summary>
class Player final : public GameObject
{
public:

	enum class EnState {
		enGround,
		enFlying,
		enDiving,
		enAttack,
		enDead,
		enNumState
	};

	enum class EnAnimation {
		enRun,
		enAttack,
		enDead,
		enNumAnim
	};

	Player();
	virtual ~Player();

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

	/// <summary>
	/// プレイヤーのステートクラスを取得する
	/// </summary>
	/// <param name="state">取得したいステートの種類</param>
	/// <returns></returns>
	IPlayerState* GetState(const EnState& state) const{
		return m_stateList[static_cast<int>(state)];
	};

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition() const {
		return m_position;
	}

	void SetPosition(const Vector3& pos) {
		m_position = pos;
	}

	void SetVelocity(const Vector3& velocity) {
		m_velocity = velocity;
	}

	const Vector3 GetVelocity() const {
		return m_velocity;
	}

	void SetLocalVelocity(const Vector3& velocity) {
		m_localVelocity = velocity;
	}

	const Vector3 GetLocalVelocity() const {
		return m_localVelocity;
	}

	void PlayAnimation(const EnAnimation& anim){
		m_model->Play(static_cast<int>(anim));
	}

	const float GetCurrentHP() {
		return m_hp;
	}

	const float GetMaxHP() {
		return m_HP_MAX;
	}

	const float GetCurrentEndurance() {
		return m_endurance;
	}

	const float GetMaxEndurance() {
		return m_ENDURANCE_MAX;
	}

	/// <summary>
	/// プレイヤーにダメージを適用する
	/// </summary>
	/// <param name="damageAmount"> 攻撃力</param>
	void ApplyDamage(const float damageAmount) {
		m_hp = max(0.f, m_hp - damageAmount);
	}

	/*
	void SetVelocityGoal(Vector3& velocityGoal) {
		m_velocityGoal = velocityGoal;
	}

	const Vector3 GetVelocityGoal() const {
		return m_velocityGoal;
	}
	*/

	/// <summary>
	/// プレイヤーの回転を取得
	/// </summary>
	/// <returns>プレイヤーに適応されるクォータニオン</returns>
	const Quaternion& GetRotation() const {
		return m_rotation;
	}

	void SetRotation(const Quaternion& rot) {
		m_rotation = rot;
	}

	const float GetSpeed() const {
		return m_speed;
	}

	IEnemy* GetTargetEnemy() const {
		return m_target;
	}

	const float GetCurrentBoost() {
		return m_boost;
	}

	const float GetMaxBoost() {
		return m_BOOST_MAX;
	}

	void UseBoost(const float amount) {
		m_boost = max(0.f, m_boost - amount);
	}

	void ChargeBoost(const float amount) {
		m_boost = min(m_BOOST_MAX, m_boost + amount);
	}

	void  UseStamina(const float amount) {
		m_endurance = max(0.f, m_endurance - amount);
	}

	void ChargeEndurance(const float amount) {
		m_endurance = min(m_ENDURANCE_MAX, m_endurance + amount);
	}

	//TODO : protect these member
	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_localVelocity = Vector3::Zero;
	//Vector3 m_velocityGoal = Vector3::Zero;

private:
	//func
	void SearchTarget();

	//Ref
	IEnemy* m_target = nullptr;

	//State
	IPlayerState* m_nextState = nullptr;
	IPlayerState* m_currentState = nullptr;
	std::vector<IPlayerState*> m_stateList;

	//Model
	ModelRender* m_model = nullptr;
	CAnimationClip m_animClip[1];
	std::vector<CAnimationClipPtr> m_animlist;
	Vector3 m_position = { 0,15,-20 };
	Quaternion m_rotation = Quaternion::Identity;
	const float m_scale = 0.05f;

	//Move
	CharacterController m_charaCon;
	float m_speed = 300.f;

	//Physics
	float m_charaConRadius = 1.f;
	float m_charaConHeight = 3.f;

	//Ability
	const float m_HP_MAX = 500.f;
	const float m_ENDURANCE_MAX = 300.f;
	const float m_BOOST_MAX = 300.f;

	float m_hp = m_HP_MAX;
	//TODO : change the name endurance to stamina
	float m_endurance = m_ENDURANCE_MAX;
	float m_boost = m_BOOST_MAX;

};