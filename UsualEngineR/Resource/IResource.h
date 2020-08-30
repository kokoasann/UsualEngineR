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
		bool m_isLoaded = false;	//読み込み完了フラグ。
		std::unique_ptr< std::thread > m_loadThread;
	};
}