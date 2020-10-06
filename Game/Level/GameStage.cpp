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
			mid.m_fxFilePath = "Assets/shader/NoAnimModel.fx";
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
					return true;
				});
			//m_isEndLoad = true;
		});

	m_threadForCreatingMeshCol.Execute([&]() {
		ModelInitData mid;
		mid.m_upAxis = EUpAxis::enUpAxisZ;
		mid.m_fxFilePath = "Assets/shader/NoAnimModel.fx";
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
				m_mapPSOList[m_count].CreateMeshObject(model, objData.position, objData.rotation, objData.scale * m_levelScale);
				m_count++;
				return true;
			});
		});

	return true;
}


void GameStage::PreUpdate()
{

}

void GameStage::Update()
{

}

void GameStage::PostUpdate()
{

}



