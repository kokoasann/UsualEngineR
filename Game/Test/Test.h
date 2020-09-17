#pragma once

class ThreadTest;
class Test:public GameObject
{
public:
	virtual void Release() override;
	virtual void OnDestroy() override;
	void Awake() override;
private:
	ThreadTest* m_threadTest = nullptr;
	
};