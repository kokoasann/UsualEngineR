#pragma once



namespace UER
{
	class ThreadObject
	{
	public:
		ThreadObject();
		~ThreadObject();


		void Execute(std::function<void()> func);

		bool IsEnd() const
		{
			return m_isEnd;
		}
	private:
		std::thread* m_thread;

		bool m_isEnd = false;
	};
}