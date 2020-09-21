#pragma once

#include "Thread/ThreadObject.h"
#include "Thread/Lock.h"

class ThreadTest:public GameObject
{
public:
	// GameObject を介して継承されました
	virtual void Release() override;

	virtual void OnDestroy() override;

	void Awake() override;
	bool Start() override;
	void Update() override;
private:
	std::unique_ptr<std::thread> m_thread;
	std::thread* m_thread_2 = nullptr;
	ModelRender* m_model;
	ModelRender* m_model_1;
	ModelRender* m_model_2;


	ThreadObject m_threadObj;				//お手軽なスレッド体験ができるクラス 詳細は ThreadObject.h まで

	int count = 0;							//変数の排他制御のテスト用。
	Lock<int> gCount = Lock<int>(count);	//変数の排他制御のテスト用。詳細は Lock.h まで

	bool m_isOK = false;
	bool m_isEndThred = false;
};