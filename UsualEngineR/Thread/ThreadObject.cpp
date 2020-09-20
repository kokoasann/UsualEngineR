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
	}
	void ThreadObject::Execute(std::function<void()> func)
	{
		m_func = func;
		m_thread = new std::thread([&]()
			{
				m_func();
				m_isEnd = true;
			});
	}
}