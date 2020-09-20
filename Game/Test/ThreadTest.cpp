#include "stdafx.h"
#include "ThreadTest.h"



void ThreadTest::Release()
{
	if (m_thread.get() != nullptr)
	{
		m_thread->join();
		
	}
	if (m_thread_2)
	{
		m_thread_2->join();
		delete m_thread_2;
	}
}

void ThreadTest::OnDestroy()
{
}

bool ThreadTest::Start()
{
	m_thread = std::make_unique < std::thread>([&]() 
		{
			auto cont = gCount.Get();
			cont.Get() += 3;
			//Sleep(5000);
			
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

	m_thread_2 = new std::thread([&]()
		{
			auto c = gCount.Get();
			c.Get() += 4;

			ModelInitData mid;
			mid.m_fxFilePath = "Assets/shader/NoAnimModel.fx";
			mid.m_vsEntryPointFunc = "VSMain";
			mid.m_psEntryPointFunc = "PSMain";
			mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";
			
			m_model_1 = NewGO<ModelRender>(0);
			m_model_1->Init(mid);
			m_model_1->SetScale(g_vec3One * 1.5);
		});

	m_threadObj.Execute([&]()
		{
			auto c = gCount.Get();
			c.Get() += 2;

			ModelInitData mid;
			mid.m_fxFilePath = "Assets/shader/NoAnimModel.fx";
			mid.m_vsEntryPointFunc = "VSMain";
			mid.m_psEntryPointFunc = "PSMain";
			mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";

			m_model_2 = NewGO<ModelRender>(0);
			m_model_2->Init(mid);
			m_model_2->SetScale(g_vec3One * 1.7);
		});
	return true;
}

void ThreadTest::Update()
{

	if (m_threadObj.IsEnd())
	{
		//終わったので開放。テストだからやっているけど、べつにここまで神経質にやる必要はない。
		m_threadObj.Release();
	}

	if (m_isOK && !m_isEndThred)
	{
		m_thread->detach();
		m_thread.release();
		m_thread = nullptr;
		m_isEndThred = true;
	}
}
