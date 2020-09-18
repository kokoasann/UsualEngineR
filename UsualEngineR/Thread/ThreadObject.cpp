#include "PreCompile.h"
#include "ThreadObject.h"


namespace UER
{
	ThreadObject::ThreadObject()
	{
	}
	ThreadObject::~ThreadObject()
	{
		m_thread->join();
		delete m_thread;
	}
	void ThreadObject::Execute(std::function<void()> func)
	{
		m_thread = new std::thread([&]()
			{
				func();
				m_isEnd = true;
			});
	}
}