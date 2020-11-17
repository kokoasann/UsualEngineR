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
		enAttackSlash,
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
	virtual void Execute() {};
	virtual void Terminate() {};
	virtual  Player::EnAttackType GetDropID() { return Player::EnAttackType::enNone; }
	virtual void PlayAnimation(const EnAnimation& anim, const float interpolate = 0.2f) {}

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

		void InitHP(const float init_hp) {
			hp = max_hp = init_hp;
		}
	};

	IEnemyState* GetState(IEnemy::EnState state) const{
		if (m_stateList.size() == 0) return nullptr;
		return m_stateList[static_cast<int>(state)];
	}

	const IEnemyState* GetCurrentState() const{
		return m_currentState;
	}

	void SetPosition(Vector3& pos) 
	{
		m_position = pos;
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

protected:
	//キャラコン作成
	void InitCharacon(const float radius, const float height, const Vector3& pos, const bool isUseRigidBody);

	void SetState(IEnemyState* s);
	bool m_isDrawHpBarAboveMyself = true;
	std::vector<IEnemyState*> m_stateList;
	Vector3 m_position;
	StAbility m_ability;

private:

	HealthBar* m_healthBar = nullptr;

	//State
	IEnemyState* m_currentState = nullptr;
	IEnemyState* m_nextState = nullptr;

	//Physics
	CharacterController m_charaCon;
	Vector3 m_velocity = Vector3::Zero;

};

