#pragma once

#include "Thread/ThreadObject.h"
#include "Thread/Lock.h"

class ThreadTest:public GameObject
{
public:
	// GameObject ����Čp������܂���
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


	ThreadObject m_threadObj;				//����y�ȃX���b�h�̌����ł���N���X �ڍׂ� ThreadObject.h �܂�

	int count = 0;							//�ϐ��̔r������̃e�X�g�p�B
	Lock<int> gCount = Lock<int>(count);	//�ϐ��̔r������̃e�X�g�p�B�ڍׂ� Lock.h �܂�

	bool m_isOK = false;
	bool m_isEndThred = false;
};