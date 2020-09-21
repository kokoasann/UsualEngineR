#pragma once


namespace UER
{
	class IResource
	{
	public:
		virtual void LoadImplement(const char* path) = 0;
		void Load(const char* path);
		void LoadAsync(const char* path);


		bool IsLoaded() const
		{
			return m_isLoaded;
		}
	private:
		void EndLoad()
		{
			m_isLoaded = true;
		}
	private:
		std::string m_filePath;
		bool m_isLoaded = false;	//“Ç‚İ‚İŠ®—¹ƒtƒ‰ƒOB
		std::unique_ptr< std::thread > m_loadThread;
		ThreadObject m_threadObj;
	};
}