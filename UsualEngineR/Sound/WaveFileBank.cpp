/*!
 *@brief	波形データバンク。
 */

#include "PreCompile.h"
#include "WaveFile.h"
#include "WaveFileBank.h"

namespace UER{
	CWaveFileBank::CWaveFileBank()
	{
	}
	CWaveFileBank::~CWaveFileBank()
	{
		ReleaseAll();
	}
	void CWaveFileBank::Release(int groupID)
	{
		for (auto waveFile : m_waveFileMap[groupID]) {
			waveFile.second->Release();
		}
		m_waveFileMap[groupID].clear();
	}
	void CWaveFileBank::UnregistWaveFile(int groupID, CWaveFilePtr waveFile)
	{
		UER_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
		m_waveFileMap->erase(waveFile->GetFilePathHash());
	}
	void CWaveFileBank::RegistWaveFile(int groupID, CWaveFilePtr waveFile)
	{
		UER_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
		m_waveFileMap[groupID].insert({ waveFile->GetFilePathHash(), waveFile });
	}
	CWaveFilePtr CWaveFileBank::FindWaveFile(int groupID, const wchar_t* filePath)
	{
		UER_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
		auto value = m_waveFileMap[groupID].find(Util::MakeHash(filePath));
		if (value != m_waveFileMap[groupID].end()) {
			return value->second;
		}
		return CWaveFilePtr();
	}
	/*CWaveFilePtr CWaveFileBank::FindWaveFile(int groupID, const WNameKey& nameKey)
	{
		UER_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
		auto value = m_waveFileMap[groupID].find(nameKey.GetHashCode());
		if (value != m_waveFileMap[groupID].end()) {
			return value->second;
		}
		return CWaveFilePtr();
	}*/
}