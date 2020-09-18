#pragma once


namespace UER
{
	template<class T>
	class Lock
	{
	public:
		Lock() = delete;
		Lock(const Lock&) = delete;
		Lock(T* content);
		~Lock();



	private:
		T* m_content;
		bool m_isUsing = false;
	};
}