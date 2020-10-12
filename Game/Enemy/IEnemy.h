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

	enum EnState {
		enIdleState,
		enBattleState,
		enNumState
	};

	IEnemyState* GetState(EnState state) {
		return m_stateList[state];
	}

	void SetPosition(Vector3& pos) 
	{
		m_position = pos;
	}

	const Vector3& GetPosition() const {
		return m_position;
	}

	const int GetHP() {
		return m_HP;
	}

	void ApplyDamage(const float damage) {
		m_HP -= damage;
	}

protected:

	void SetState(IEnemyState* s);

	std::vector<IEnemyState*> m_stateList;
	Vector3 m_position;

	float m_HP = 100.f;

private:
	IEnemyState* m_currentState = nullptr;
	IEnemyState* m_nextState = nullptr;

};

