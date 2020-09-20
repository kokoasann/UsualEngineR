#pragma once


namespace UER
{
	class GameTime
	{
	
		GameTime() {}
		~GameTime(){}
	public:

		static GameTime* Get()
		{
			static GameTime* inst = nullptr;
			if (inst == nullptr)
				inst = new GameTime();
			return inst;
		}

		void PushFrameDeltaTime(float time)
		{
			//m_deltaTimeQue.push_back(time);

			//float Qsize = m_deltaTimeQue.size();
			//if (Qsize > 30)
			//{
			//	float totalTime = 0;
			//	for (auto dt : m_deltaTimeQue)
			//	{
			//		totalTime += dt;
			//	}
			//	mFrameDeltaTime = min(1.f / 30.f, totalTime / Qsize);
			//	
			//	//m_deltaTimeQue.shrink_to_fit();
			//	//m_deltaTimeQue.erase(m_deltaTimeQue.begin());
			//	m_deltaTimeQue.pop_front();
			//	
			//}

			m_deltaTimeArray[m_queueCount] = time;
			if (m_queueCount + 1 >= MAX_QUEUE)
			{
				float totalTime = 0;
				for (auto dt : m_deltaTimeArray)
				{
					totalTime += dt;
				}
				mFrameDeltaTime = min(1.f / 30.f, totalTime / (m_queueCount + 1));

				for (int i =1;i< (m_queueCount + 1);i++)
				{
					m_deltaTimeArray[i - 1] = m_deltaTimeArray[i];
				}
			}
			else
			{
				m_queueCount++;
			}
		} 

		float GetDeltaTime()
		{
			return mFrameDeltaTime;
		}

	private:
		static const int MAX_QUEUE = 30;
		std::list<float> m_deltaTimeQue;
		std::array<float, MAX_QUEUE> m_deltaTimeArray;
		int m_queueCount = 0;
		float mFrameDeltaTime = 1.f / 30.f;
	};

	static GameTime* gameTime()
	{
		return GameTime::Get();
	}
}