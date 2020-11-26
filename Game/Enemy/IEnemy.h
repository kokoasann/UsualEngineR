#pragma once
class IEnemyState;
class HealthBar;
#include "../../UsualEngineR/Character/CharacterController.h"
#include "../Player/Player.h"

class IEnemy : public GameObject
{
public:

	enum class EnState {
		enIdleState,
		enBattleState,
		enAttackA,
		enAttackB,
		enDeadState,
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

	virtual~IEnemy() {};

	virtual void Init() {};
	virtual void InitState();
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

	IEnemyState* GetState(const int stateNo) const{
		if (m_stateMap.size() == 0) return nullptr;
		return m_stateMap.at(stateNo);
	}

	const IEnemyState* GetCurrentState() const{
		return m_currentState;
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

	void ApplyDamage(const float damage);

	void UseStamina(const float amount);

	void RecoverStamina(const float amount);

	void SetAbility(const StAbility& ability) {
		m_ability = ability;
	}

	const StAbility& GetAbility() const {
		return m_ability;
	}

	const bool IsBoss() {
		return !m_isDrawHpBarAboveMyself;
	}

	void SetVelocity(const Vector3& velocity) {
		m_velocity = velocity;
	}

	const std::map<int, IEnemyState*>& GetStateMap() const{
		return m_stateMap;
	}

	const bool IsOnGround() {
		return m_charaCon.IsOnGround();
	}

	const ModelRender* GetMode() const{
		return m_model;
	}

	std::vector<JetEffect*>& GetJetEffects() {
		return m_jetEffects;
	}

	void SetRotation(const Quaternion& rot) {
		m_rotation = rot;
	}

	const Quaternion& GetRotation() {
		return m_rotation;
	}

protected:
	//キャラコン作成
	void InitCharacon(const float radius, const float height, const Vector3& pos, const bool isUseRigidBody);

	void SetState(IEnemyState* s);
	bool m_isDrawHpBarAboveMyself = true;

	//std::vector<IEnemyState*> m_stateList;

	std::map<int, IEnemyState*> m_stateMap;
	std::map<int, std::unique_ptr<CAnimationClip>> m_animationMap;

	void SetAnimation(const int no, const char* name, const bool loopFlag) {
		m_animationMap.insert(std::make_pair(no, std::make_unique<CAnimationClip>()));
		m_animationMap.at(no)->Load(name);
		m_animationMap.at(no)->BuildKeyFramesAndAnimationEvents();
		m_animationMap.at(no)->SetLoopFlag(loopFlag);
	}

	ModelRender* m_model = nullptr;
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	StAbility m_ability;

	//Effects
	std::vector<Bone*> m_bones;
	std::vector<JetEffect*> m_jetEffects;

	//Physics
	CharacterController m_charaCon;
	Vector3 m_velocity = Vector3::Zero;

private:

	HealthBar* m_healthBar = nullptr;

	//State
	IEnemyState* m_currentState = nullptr;
	IEnemyState* m_nextState = nullptr;


};

