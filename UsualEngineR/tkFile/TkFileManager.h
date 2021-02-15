#pragma once


namespace UER
{
	
	template<class tkFile>
	class TkFileManager
	{
	public:
		TkFileManager()
		{

		}
		virtual ~TkFileManager()
		{

		}
		void Release()
		{
			for (auto& p : m_resources)
			{
				delete p.second.file;
			}
		}

		tkFile* Load(const char* path)
		{
			tkFile* res = nullptr;
			int hash = Util::MakeHash(path);
			load_mutex.lock();
			auto it = m_resources.find(hash);
			if (it == m_resources.end())
			{
				FileData fdata;
				m_resources.insert(std::make_pair(hash, fdata));
				load_mutex.unlock();

				tkFile* tk = new tkFile;
				tk->Load(path);
				m_resources.at(hash).file = tk;

				res = tk;
				m_resources.at(hash).isLoaded = true;
			}
			else
			{
				load_mutex.unlock();
				const FileData& fd = m_resources.at(hash);
				while(!fd.isLoaded)
				{
					Sleep(10);
				}
				res = fd.file;
			}

			return res;
		}

	private:
		static std::mutex load_mutex;

		struct FileData
		{
			tkFile* file = 0;
			bool isLoaded = false;
		};

		std::map<int, FileData> m_resources;
		
		
	};
	template<class tkFile>
	std::mutex TkFileManager<tkFile>::load_mutex;
}