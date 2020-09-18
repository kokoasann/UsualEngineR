#include "stdafx.h"
#include "ThreadTest.h"

void ThreadTest::Release()
{
	if (m_thread.get() != nullptr)
	{
		m_thread->detach();
	}
}

void ThreadTest::OnDestroy()
{
}

bool ThreadTest::Start()
{
	m_thread = std::make_unique < std::thread>([&]() 
		{
			Sleep(5000);
			ModelInitData mid;
			mid.m_fxFilePath = "Assets/shader/AnimModel.fx";
			mid.m_vsEntryPointFunc = "VSMain";
			mid.m_psEntryPointFunc = "PSMain";
			mid.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
			mid.m_tksFilePath = "Assets/modelData/unityChan.tks";
			m_model = NewGO<ModelRender>(0);
			m_model->Init(mid);
			m_model->SetScale(g_vec3One * 0.05*1.5);
			m_isOK = true;
		});
	return true;
}

void ThreadTest::Update()
{
	if (m_isOK && !m_isEndThred)
	{
		m_thread->detach();
		m_thread.release();
		m_thread = nullptr;
		m_isEndThred = true;
	}
}
