#pragma once
class IEnemyState;

class IEnemy : public GameObject
{
public:
	virtual~IEnemy() {};

	virtual void Init() {};
	virtual void Execute() {};
	virtual void Terminate() {};

	virtual void Awake() override final;
	virtual bool Start() override final;
	virtual void Update() override final;
	virtual void Release() override final;
	virtual void OnDestroy() override final;

	//パス検索とか重めの共通処理
	void PathFind(Vector3 start, Vector3 end) { Sleep(1000); };

	enum class EnState {
		enIdleState,
		enBattleState,
		enAttackSlash,
		enDeadState,
		enNumState
	};

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

	void ApplyDamage(const float damage) {
		m_ability.hp = max(0.f, m_ability.hp - damage);
	}

	void SetAbility(const StAbility& ability) {
		m_ability = ability;
	}

	const StAbility& GetAbility() const {
		return m_ability;
	}

protected:

	void SetState(IEnemyState* s);

	std::vector<IEnemyState*> m_stateList;
	Vector3 m_position;

	//const float m_HP_MAX = 100.f;
	//float m_hp = m_HP_MAX;

	StAbility m_ability;

private:
	IEnemyState* m_currentState = nullptr;
	IEnemyState* m_nextState = nullptr;
};

