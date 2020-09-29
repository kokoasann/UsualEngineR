#include "PreCompile.h"
#include "TextureManager.h"




namespace UER
{
	TextureManager::~TextureManager()
	{
		Release();
	}
	void TextureManager::Release()
	{
		for (auto d : m_textureDic)
		{
			d.second.texture->Release();
		}
		m_textureDic.clear();
	}
	ID3D12Resource* TextureManager::Load(const wchar_t* path)
	{
		ID3D12Resource* res = nullptr;
		int hash = Util::MakeHash(path);
		m_loadMutex.lock();
		auto it = m_textureDic.find(hash);
		if (it == m_textureDic.end())
		{
			FileData fdata;
			m_textureDic.insert(std::make_pair(hash, fdata));
			m_loadMutex.unlock();

			auto device = g_graphicsEngine->GetD3DDevice();
			DirectX::ResourceUploadBatch re(device);
			re.Begin();
			ID3D12Resource* texture;
			auto hr = DirectX::CreateDDSTextureFromFileEx(
				device,
				re,
				path,
				0,
				D3D12_RESOURCE_FLAG_NONE,
				0,
				&texture
			);
			re.End(g_graphicsEngine->GetCommandQueue());
			
			
			m_textureDic.at(hash).isLoaded = true;
			m_textureDic.at(hash).texture = texture;

			res = texture;
		}
		else
		{
			m_loadMutex.unlock();
			const FileData& fd = m_textureDic.at(hash);
			while (!fd.isLoaded)
			{
				Sleep(10);
			}
			res = fd.texture;
		}

		return res;
	}
	ID3D12Resource* TextureManager::Load(const char* memory, UINT size, const wchar_t* path)
	{
		ID3D12Resource* res = nullptr;
		int hash = Util::MakeHash(path);
		m_loadMutex.lock();
		auto it = m_textureDic.find(hash);
		if (it == m_textureDic.end())
		{
			FileData fdata;
			m_textureDic.insert(std::make_pair(hash, fdata));
			m_loadMutex.unlock();

			auto device = g_graphicsEngine->GetD3DDevice();
			DirectX::ResourceUploadBatch re(device);
			re.Begin();
			ID3D12Resource* texture;
			auto hr = DirectX::CreateDDSTextureFromMemoryEx(
				device,
				re,
				(const uint8_t*)memory,
				size,
				0,
				D3D12_RESOURCE_FLAG_NONE,
				0,
				&texture
			);
			re.End(g_graphicsEngine->GetCommandQueue());

			if (FAILED(hr)) {
				//テクスチャの作成に失敗しました。
				return nullptr;
			}


			m_textureDic.at(hash).isLoaded = true;
			m_textureDic.at(hash).texture = texture;

			res = texture;
		}
		else
		{
			m_loadMutex.unlock();
			const FileData& fd = m_textureDic.at(hash);
			while (!fd.isLoaded)
			{
				Sleep(10);
			}
			res = fd.texture;
		}

		return res;
	}
}