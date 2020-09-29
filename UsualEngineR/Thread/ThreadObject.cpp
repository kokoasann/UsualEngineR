#include "PreCompile.h"
#include "ThreadObject.h"


namespace UER
{
	ThreadObject::ThreadObject()
	{
	}
	ThreadObject::~ThreadObject()
	{
		Release();
	}
	void ThreadObject::Release()
	{
		if (!m_thread)
			return;
		m_thread->join();
		delete m_thread;
		m_thread = nullptr;

		m_isExecuted = false;
	}
	void ThreadObject::Execute(const std::function<void()>& func)
	{
		if (m_isExecuted)
		{
			//ThreadObject‚ÍŽg‚¢‚Ü‚í‚µ‚Å‚«‚Ü‚¹‚ñ!
			MessageBoxW(nullptr, L"error", L"THREAD ERROR", MB_OK);
			std::abort();
			return;
		}

		m_func = func;
		m_thread = new std::thread([&]()
			{
				m_func();
				m_isEnd = true;
			});

		m_isExecuted = true;
	}
}