#include "PreCompile.h"
#include "IResource.h"

namespace UER
{
	void IResource::Load(const char* path)
	{
		m_isLoaded = true;
		m_filePath = path;
		LoadImplement(path);
	}
	void IResource::LoadAsync(const char* path)
	{
		m_filePath = path;
		//m_loadThread = std::make_unique<std::thread>([&]() {LoadImplement(m_filePath.c_str()); });
		m_threadObj.Execute([&]() 
			{
				LoadImplement(m_filePath.c_str());
				m_isLoaded = true;
			});
	}
}