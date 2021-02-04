#pragma once
class IEnemyState;
class HealthBar;
class ExplosionEffect;
class SmokeEffect;

#include "../../UsualEngineR/Character/CharacterController.h"
#include "../Player/Player.h"
#include "Effect/SmokeEffect.h"

class IEnemy : public GameObject
{
public:

	enum class EnState {
		enIdleState,
		enBattleState,
		enAttackA,
		enAttackB,
		enDeadState,
		enStunState,
		enNumState
	};

	enum class EnAnimation {
		enIdle,
		enWalk,
		enRun,
		enAttackA,
		enAttackB,
		enNumAnim
	};

	enum class EnIK
	{
		enHead,
		enChest,
		enArm_L,
		enArm_R,
		enFoot_L,
		enFoot_R,
		enNumIK
	};

	virtual~IEnemy() {};

	virtual void Init() {};
	virtual void InitState();
	virtual void InitIK() {};
	virtual void Execute() {};
	virtual void Terminate() {};
	virtual  Player::EnAttackType GetDropID() { return Player::EnAttackType::enNone; }
	virtual void PlayAnimation(const EnAnimation& anim, const float interpolate = 0.2f) {}
	virtual void PlayAnimation(const int animNo, const float interpolate = 0.2f) {}

	virtual void Awake() override final;
	virtual bool Start() override final;
	virtual void Update() override final;
	virtual void Release() override final;
	virtual void OnDestroy() override final;

	void RemoveRigidBody()
	{
		m_charaCon.RemoveRigidBoby();
	}

	//パス検索とか重めの共通処理
	void PathFind(Vector3 start, Vector3 end) { Sleep(1000); };

	//TODO : データを構造体にまとめる
	/*
	struct EnemyData {
		float hp;
		float currentHp;
		bool isBoss;
		Vector3 pos;
	};
	*/

	struct StAbility {
		float hp;
		float max_hp;
		float stamina;
		float max_stamina;

		void InitHP(const float init_hp) {
			hp = max_hp = init_hp;
		}

		void InitStamina(const float init_stamina) {
			stamina = max_stamina = init_stamina;
		}
	};

	IEnemyState* GetState(const int stateNo) const {
		if (m_stateMap.size() == 0) return nullptr;

		auto it = m_stateMap.find(stateNo);
#ifdef DEBUG
		if (it == m_stateMap.end()) {
			assert(false, "取得しようとしたステートが無いんですけどっ！！");
		}
#endif // DEBUG

#ifndef DEBUG
		if (it == m_stateMap.end()) {
			return m_stateMap.at(TO_INT(IEnemy::EnState::enIdleState));
		}
#endif // !DEBUG

		return it->second;
	}

	/// <summary>
	/// IKをゲット
	/// ない場合はnullptrを返す
	/// </summary>
	/// <param name="ikKind">EnIKとか</param>
	/// <returns></returns>
	IK* GetIK(const int ikKind) const
	{
		if (m_ikMap.size() == 0) return nullptr;

		auto it = m_ikMap.find(ikKind);

		if (it == m_ikMap.end()) {
			return nullptr;
		}

		return it->second;
	}

	const IEnemyState* GetCurrentState() const{
		return m_currentState;
	}

	IEnemyState* GetPreviousState() const {
		return m_previousState;
	}

	void SetPosition(const Vector3& pos) 
	{
		m_position = pos;
		m_charaCon.SetPosition(pos);
	}

	const Vector3& GetPosition() const {
		return m_position;
	}

	const float GetCurrentHP() {
		return m_ability.hp;
	}
	
	const float GetMaxHP() {
		return m_ability.max_hp;
	}

	void ApplyDamage(const float damage, const bool stunFlag = false, const Vector3& impulse = Vector3::Zero);

	void UseStamina(const float amount);

	void RecoverStamina(const float amount);

	void SetAbility(const StAbility& ability) {
		m_ability = ability;
	}

	const StAbility& GetAbility() const {
		return m_ability;
	}

	const bool IsBoss() {
		return !m_isNormalEnemy;
	}

	void SetVelocity(const Vector3& velocity) {
		m_velocity = velocity;
	}

	Vector3& GetVelocity() {
		return m_velocity;
	}

	const Vector3& GetKnockBackImpulse() {
		return m_impulse;
	}

	void SetKnockBackImpulse(const Vector3& imp) {
		m_impulse = imp;
	}

	const std::map<int, IEnemyState*>& GetStateMap() const{
		return m_stateMap;
	}

	const bool IsOnGround() {
		return m_charaCon.IsOnGround();
	}

	ModelRender* GetModel() const{
		return m_model;
	}

	std::vector<JetEffect*>& GetJetEffects() {
		return m_jetEffects;
	}

	void PlayJetSE() {
		if (m_jetSE != nullptr) {
			m_jetSE->Play(true);
		}
	}

	void StopJetSE() {
		if (m_jetSE != nullptr)
			m_jetSE->Stop();
	}

	void SetRotation(const Quaternion& rot) {
		m_rotation = rot;
	}

	const Quaternion& GetRotation() {
		return m_rotation;
	}

	Vector3 GetForward() {
		auto& world = m_model->GetModel().GetWorldMatrix();
		Vector3 forward;
		forward.x = world.mat._31;
		forward.y = world.mat._32;
		forward.z = world.mat._33;
		forward.Normalize();
		return forward;
	}

	const float GetCharaRadius() {
		return m_radius;
	}

	virtual void Performance() {}

	void SetAutoRotateFlag(const bool flag) {
		m_isUseAutoRotateSystem = flag;
	}

	virtual void Explode() {};

	virtual void SetSmokeFlag(const bool flag) {

		if (flag) {
			m_smokeEffect->SetPos(m_position);
			m_smokeEffect->SetSca(Vector3::One * 0.5f);
			m_smokeEffect->Play();
		}
		else {
			m_smokeEffect->Stop();
		}

	}

	void SetExternalVelocity(const Vector3& velocity) {
		m_externalVelocity = velocity;
	}

	float GetCharaconHeghit() const
	{
		return m_charaCon.GetHeight();
	}

	float GetCharaconRadius() const
	{
		return m_charaCon.GetRadius();
	}
protected:
	//キャラコン作成
	void InitCharacon(const float radius, const float height, const Vector3& pos, const bool isUseRigidBody);

	void SetState(IEnemyState* s);
	bool m_isNormalEnemy = true;

	std::map<int, IEnemyState*> m_stateMap;
	std::map<int, std::unique_ptr<CAnimationClip>> m_animationMap;
	std::map<int, IK*> m_ikMap;

	void SetAnimation(const int no, const char* path, const bool loopFlag) {
		m_animationMap.insert(std::make_pair(no, std::make_unique<CAnimationClip>()));
		m_animationMap.at(no)->Load(path);
		m_animationMap.at(no)->BuildKeyFramesAndAnimationEvents();
		m_animationMap.at(no)->SetLoopFlag(loopFlag);
	}

	void SetIK(const int no, IK* ik)
	{
		m_ikMap.insert(std::make_pair(no, ik));
	}

	ModelRender* m_model = nullptr;
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	StAbility m_ability;

	//Effects
	std::vector<Bone*> m_bones;
	std::vector<JetEffect*> m_jetEffects;
	ExplosionEffect* m_explodeEffect = nullptr;
	SmokeEffect* m_smokeEffect = nullptr;
	Vector4 m_smokeColor = { 1.f,1.f,1.f,0.1f };

	//SE
	CSoundSource* m_jetSE = nullptr;

	//Physics
	CharacterController m_charaCon;
	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_externalVelocity = Vector3::Zero; //外部からの影響
	Vector3 m_impulse = Vector3::Zero;
	float m_radius = 1.f;



private:

	HealthBar* m_healthBar = nullptr;

	//State
	IEnemyState* m_currentState = nullptr;
	IEnemyState* m_previousState = nullptr;
	IEnemyState* m_nextState = nullptr;


	bool m_isUseAutoRotateSystem = true;

	bool m_addedAsDeadBody = false;

	const int m_ResetPos = 7.f;
};

