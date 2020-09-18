#pragma once

#include "ThreadObject.h"

namespace UER
{
	class ThreadManager
	{
	private:
		ThreadManager();
		~ThreadManager();
	public:
		ThreadManager& Get()
		{
			static ThreadManager tm;
			return tm;
		}



		void AddThreadObj(ThreadObject* obj)
		{
			m_threadList.push_back(obj);
		}

	private:
		std::vector<ThreadObject*> m_threadList;
	};
}