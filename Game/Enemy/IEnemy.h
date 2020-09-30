#pragma once
class IEnemyState;

class IEnemy : public GameObject
{
public:
	virtual~IEnemy() {};
	virtual bool Start();
	virtual void Update();
	virtual void Release();
	virtual void OnDestroy();

	//�p�X�����Ƃ��d�߂̋��ʏ���
	void PathFind(Vector3 start, Vector3 end) { Sleep(1000); };
protected:

	void SetState(IEnemyState* s);

private:
	IEnemyState* m_currentState = nullptr;
	IEnemyState* m_nextState = nullptr;
};

