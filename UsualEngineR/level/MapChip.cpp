#include "PreCompile.h"
#include "MapChip.h"
#include "Level.h"

namespace UER
{
	MapChip::MapChip(const LevelObjectData& objData)
	{
		std::wstring wname = objData.name;
		std::string name(wname.begin(), wname.end());
		char filePath[256];
		sprintf_s(filePath, "Assets/modelData/%s.tkm", name.c_str());
		ModelInitData mid;
		mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
		mid.m_vsEntryPointFunc = "VSMain";
		mid.m_psEntryPointFunc = "PSMain";
		mid.m_tkmFilePath = filePath;

		m_model.Init(mid);
		m_model.UpdateWorldMatrix(objData.position, objData.rotation, objData.scale);
		//静的物理オブジェクトをメッシュコライダーから作成する。
		m_physicsStaticObject.CreateMeshObject(m_model, objData.position, objData.rotation,objData.scale);
	}

	void MapChip::Draw()
	{
		m_model.Draw(g_graphicsEngine->GetRenderContext());
	}
}