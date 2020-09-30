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

	//ƒpƒXŒŸõ‚Æ‚©d‚ß‚Ì‹¤’Êˆ—
	void PathFind(Vector3 start, Vector3 end) { Sleep(1000); };


	enum EnState {
		enIdleState,
		enBattleState,
		enNumState
	};

	IEnemyState* GetState(EnState state) {
		return m_stateList[state];
	}

protected:

	void SetState(IEnemyState* s);

	std::vector<IEnemyState*> m_stateList;

private:
	IEnemyState* m_currentState = nullptr;
	IEnemyState* m_nextState = nullptr;
};

