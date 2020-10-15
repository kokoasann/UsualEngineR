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

	//ÉpÉXåüçıÇ∆Ç©èdÇﬂÇÃã§í èàóù
	void PathFind(Vector3 start, Vector3 end) { Sleep(1000); };

	enum class EnState {
		enIdleState,
		enBattleState,
		enAttackSlash,
		enDeadState,
		enNumState
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
		return m_hp;
	}
	
	const float GetMaxHP() {
		return m_HP_MAX;
	}

	void ApplyDamage(const float damage) {
		m_hp = max(0.f, m_hp - damage);
	}

protected:

	void SetState(IEnemyState* s);

	std::vector<IEnemyState*> m_stateList;
	Vector3 m_position;

	const float m_HP_MAX = 100.f;
	float m_hp = m_HP_MAX;

private:
	IEnemyState* m_currentState = nullptr;
	IEnemyState* m_nextState = nullptr;

};

