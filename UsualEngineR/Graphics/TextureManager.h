#pragma once



namespace UER
{
	class TextureManager
	{
		TextureManager() {};
	public:
		virtual ~TextureManager();

		static TextureManager& Get()
		{
			static TextureManager tm;
			return tm;
		}

		void Release();

		ID3D12Resource* Load(const wchar_t* path);

		ID3D12Resource* Load(const char* memory, UINT size, const wchar_t* path);


	private:
		struct FileData
		{
			bool isLoaded = false;
			ID3D12Resource* texture = nullptr;
		};
		std::map<int, FileData> m_textureDic;

		std::mutex m_loadMutex;
	};
}