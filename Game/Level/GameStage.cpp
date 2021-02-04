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
	DeleteGO(m_sandSmoke);
}

void GameStage::OnDestroy()
{
	Release();
}


void GameStage::Awake()
{

}
static std::mutex RigidBodyMutex;
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
			mid.m_psEntryPointFunc = "PSMain_nor";
			//m_level.Init("Assets/level/map_level.tkl", [&](LevelObjectData& objData)->bool
			m_level.Init("Assets/level/map_commit_level.tkl", [&](LevelObjectData& objData)->bool
				{
					std::string name(objData.name.begin(), objData.name.end());
					char filePath[256];
					//sprintf_s(filePath, "Assets/modelData/map/%s.tkm", name.c_str());
					sprintf_s(filePath, "Assets/modelData/map/commit/%s.tkm", name.c_str());
					ModelRender* mr = NewGO<ModelRender>(0);
					mr->SetScale(Vector3::One * m_levelScale);
					mid.m_tkmFilePath = filePath;
					mr->Init(mid);
					m_mapmodel.push_back(mr);
					//mr->SetMulColor({ 0.8,0.7,0.3,1 });
					mr->SetMulColor({ 0.4,0.4,0.4,1 });
					return true;
				});

			{
				/*mid.m_upAxis = EUpAxis::enUpAxisY;
				m_mapDeco[0] = NewGO<ModelRender>(0);
				m_mapDeco[0]->SetScale(Vector3::One * m_levelScale);
				mid.m_tkmFilePath = "Assets/modelData/map_deco/map_middle.tkm";
				m_mapDeco[0]->Init(mid);
				m_mapDeco[0]->SetMulColor({ 0.4,0.4,0.4,1 });

				m_mapDeco[1] = NewGO<ModelRender>(0);
				m_mapDeco[1]->SetScale(Vector3::One * m_levelScale);
				mid.m_tkmFilePath = "Assets/modelData/map_deco/map_far.tkm";
				m_mapDeco[1]->Init(mid);
				m_mapDeco[1]->SetMulColor({ 0.4,0.4,0.4,1 });*/
			}

			//m_isEndLoad = true;
		});

	{
		ModelInitData mid;
		mid.m_upAxis = EUpAxis::enUpAxisY;
		mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
		mid.m_vsEntryPointFunc = "VSMain";
		mid.m_psEntryPointFunc = "PSMain";
		//m_level.Init("Assets/level/map_level.tkl", [&](LevelObjectData& objData)->bool
		m_footholdLevel.Init("Assets/level/map_commit_obj_level2.tkl", [&](LevelObjectData& objData)->bool
			{
				std::string name(objData.name.begin(), objData.name.end());
				char filePath[256];
				//sprintf_s(filePath, "Assets/modelData/map/%s.tkm", name.c_str());
				sprintf_s(filePath, "Assets/modelData/map_obj/%s.tkm", name.c_str());
				ModelRender* mr = NewGO<ModelRender>(0);
				mr->SetScale(objData.scale * m_levelScale);
				mr->SetPosition(objData.position * m_levelScale);
				mr->SetRotation(objData.rotation);
				mid.m_tkmFilePath = filePath;
				mr->Init(mid);
				m_mapmodel.push_back(mr);
				mr->SetMulColor({ 0.6,0.6,0.6,1 });


				RigidBodyMutex.lock();

				m_mapObjPSOList[m_mapObjPSOCount].CreateMeshObject(mr->GetModel(), objData.position * m_levelScale, objData.rotation, objData.scale * m_levelScale, true);
				m_mapObjPSOList[m_mapObjPSOCount].GetRigidBody().GetBody()->setUserIndex(enCollisionAttr_Ground);
				Physics().AddRigidBody(m_mapObjPSOList[m_mapObjPSOCount].GetRigidBody());
				m_mapObjPSOCount++;

				RigidBodyMutex.unlock();
				return true;
			});
	}

	m_threadForCreatingMeshCol.Execute([&]() {
		ModelInitData mid;
		mid.m_upAxis = EUpAxis::enUpAxisZ;
		mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
		mid.m_vsEntryPointFunc = "VSMain";
		mid.m_psEntryPointFunc = "PSMain";

		m_phyLevel.Init("Assets/level/map_physics_level.tkl", [&](LevelObjectData& objData)->bool
			{
				std::string name(objData.name.begin(), objData.name.end());
				char filePath[256];
				sprintf_s(filePath, "Assets/modelData/map_physics/%s.tkm", name.c_str());
				mid.m_tkmFilePath = filePath;

				Model model;
				model.Init(mid);

				RigidBodyMutex.lock();
				//スレッドでBulletにgiridBodyを積んでいくと競合する可能性があるので、ひとまずメンバにスタックしておくゾ。
				m_rigidBodys.push_back(m_mapPSOList[m_count].CreateMeshObject(model, objData.position, objData.rotation, objData.scale * m_levelScale,true));
				m_count++;
				RigidBodyMutex.unlock();
				return true;
			});
		m_isLoaded = true;
		DebugPrint_WATA("Level meash has been created");
		});

	m_decoLevel.Init("Assets/level/map_MBG_level.tkl", [&](LevelObjectData& objData)->bool
		{
			ModelInitData mid;
			mid.m_upAxis = EUpAxis::enUpAxisY;
			mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
			mid.m_vsEntryPointFunc = "VSMain";
			mid.m_psEntryPointFunc = "PSMain";
			mid.m_tkmFilePath = "Assets/modelData/m_BG/m_BG1.tkm";
			ModelRender* m = NewGO<ModelRender>(0);
			m->Init(mid);
			m->SetPosition(objData.position * m_levelScale);
			m->SetRotation(objData.rotation);
			m->SetScale(objData.scale* m_levelScale);
			m->SetMulColor({ 1,0.8,0.4,1. });
			m_mbgs.push_back(m);
			return true;
		});


	m_sandSmoke = NewGO<VolumetricEffectRender>(0);
	m_sandSmoke->Init("Assets/modelData/test/block.tkm");
	m_sandSmoke->SetPos({ 0,200,0 });
	m_sandSmoke->SetSca({ 850,200,850 });
	{
		auto& fd = m_sandSmoke->GetFogData();
		fd.rayCount = 30;
		fd.rayLen = 16.f;
		fd.concentration = 0.015*8./3.;
		fd.decayCenterToXZ = 0.0f;
		fd.decayCenterToY = 0.0f;
		fd.ratioParlinWorley = 0.f;
		fd.ratioParlin = 1.f;
		fd.perlinScale = 0.02f;
		fd.color = { .7,.65,.55 };
		//fd.ratioParlin = 
	}

	static float i = 1;
	DebugSwitchAddButton(DebugSwitchNewSwitch_Button('Z', VK_LEFT, [&]()
		{
			i*=0.5f;
			auto& fd = m_sandSmoke->GetFogData();
			fd.rayCount = 30.f * i;
			fd.rayLen = 16.f / i;
			fd.concentration = 0.015 * 8. / 3. / i;
		}));
	DebugSwitchAddButton(DebugSwitchNewSwitch_Button('Z', VK_RIGHT, [&]()
		{
			i *= 2;
			if (i > 1.f)
				i = 1.f;
			auto& fd = m_sandSmoke->GetFogData();
			fd.rayCount = 30 * i;
			fd.rayLen = 16.f / i;
			fd.concentration = 0.015 * 8. / 3. / i;
		}));


	

	return true;
}


void GameStage::PreUpdate()
{

}

void GameStage::Update()
{
	{
		auto& fd = m_sandSmoke->GetFogData();
		fd.offset.y -= 3.f * gameTime()->GetDeltaTime();
	}


	//メンバにスタックしたrigidBodyを一気にBulletに詰め込んでゆくゾ。
	if (m_threadForCreatingMeshCol.IsEnd() && !m_isRegistRigidBody)
	{
		for (auto& rb : m_rigidBodys)
		{
			Physics().AddRigidBody(*rb);
			rb->GetBody()->setUserIndex(enCollisionAttr_None);
			rb->GetBody()->setUserIndex(enCollisionAttr_Ground);
		}
		m_isRegistRigidBody = true;
	}
}

void GameStage::PostUpdate()
{

}



