#include "stdafx.h"
#include "GameStage.h"


GameStage::GameStage()
{

}

GameStage::~GameStage()
{

}



void GameStage::Release()
{
	m_threadForLevel.Release();
	std::for_each(m_mapmodel.begin(), m_mapmodel.end(), [](ModelRender* model) { DeleteGO(model); });

}

void GameStage::OnDestroy()
{
	Release();
}


void GameStage::Awake()
{

}

bool GameStage::Start()
{
	m_threadForLevel.Execute([&]()
		{
			Stopwatch sw;
			sw.Start();
			ModelInitData mid;
			mid.m_upAxis = EUpAxis::enUpAxisZ;
			mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
			mid.m_vsEntryPointFunc = "VSMain";
			mid.m_psEntryPointFunc = "PSMain";
			m_level.Init("Assets/level/map_level.tkl", [&](LevelObjectData& objData)->bool
				{
					std::string name(objData.name.begin(), objData.name.end());
					char filePath[256];
					sprintf_s(filePath, "Assets/modelData/map/%s.tkm", name.c_str());
					ModelRender* mr = NewGO<ModelRender>(0);
					mr->SetScale(Vector3::One * m_levelScale);
					mid.m_tkmFilePath = filePath;
					mr->Init(mid);
					m_mapmodel.push_back(mr);
					mr->SetMulColor({ 0.8,0.7,0.3,1 });
					return true;
				});
			//m_isEndLoad = true;
		});

	m_threadForCreatingMeshCol.Execute([&]() {
		ModelInitData mid;
		mid.m_upAxis = EUpAxis::enUpAxisZ;
		mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
		mid.m_vsEntryPointFunc = "VSMain";
		mid.m_psEntryPointFunc = "PSMain";

		m_level.Init("Assets/level/map_physics_level.tkl", [&](LevelObjectData& objData)->bool
			{
				std::string name(objData.name.begin(), objData.name.end());
				char filePath[256];
				sprintf_s(filePath, "Assets/modelData/map_physics/%s.tkm", name.c_str());
				mid.m_tkmFilePath = filePath;

				Model model;
				model.Init(mid);

				//スレッドでBulletにgiridBodyを積んでいくと競合する可能性があるので、ひとまずメンバにスタックしておくゾ。
				m_rigidBodys.push_back(m_mapPSOList[m_count].CreateMeshObject(model, objData.position, objData.rotation, objData.scale * m_levelScale,true));
				m_count++;
				return true;
			});
		DebugPrint_WATA("Level meash has been created");
		});

	return true;
}


void GameStage::PreUpdate()
{

}

void GameStage::Update()
{
	//メンバにスタックしたrigidBodyを一気にBulletに詰め込んでゆくゾ。
	if (m_threadForCreatingMeshCol.IsEnd() && !m_isRegistRigidBody)
	{
		for (auto& rb : m_rigidBodys)
		{
			Physics().AddRigidBody(*rb);
			rb->GetBody()->setUserIndex(enCollisionAttr_None);
		}
		m_isRegistRigidBody = true;
	}
}

void GameStage::PostUpdate()
{

}



