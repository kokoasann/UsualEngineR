#include "stdafx.h"
#include "Game.h"
#include "Player/Player.h"
#include "Enemy/EnemyTest.h"
#include "Camera/GameCamera.h"

void Game::Release()
{
	DeleteGO(m_player);
	std::for_each(m_mapmodel.begin(), m_mapmodel.end(), [](ModelRender* model) { DeleteGO(model); });
}

void Game::OnDestroy()
{
}

void Game::Awake()
{
	AllocConsole();
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);


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


	/*
	ModelInitData mid;
	mid.m_fxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	mid.m_tksFilePath = "Assets/modelData/unityChan.tks";
	mid.m_upAxis = enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(g_vec3One*0.05);
	m_animlist.resize(1);
	m_animlist[0] = std::make_unique<CAnimationClip>();
	m_animlist[0]->Load("Assets/anim/unityChan/run.tka");
	m_animlist[0]->BuildKeyFramesAndAnimationEvents();
	m_animlist[0]->SetLoopFlag(true);
	
	m_model->InitAnimation(m_animlist,1);
	m_model->Play(0);

	mid.m_fxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";
	mid.m_tksFilePath = nullptr;
	mid.m_upAxis = enUpAxisZ;
	m_test = NewGO<ModelRender>(0);
	m_test->Init(mid);
	m_test->SetPosition({ 0,0,0 });
	m_test->SetScale(g_vec3One);
	*/
}

bool Game::Start()
{
	m_player = NewGO<Player>(0);
	m_camera = NewGO<GameCamera>(0);
	auto tarPos = m_player->GetPosition();
	m_camera->SetTarget(tarPos);

	return true;
}

void Game::PreUpdate()
{
}

void Game::Update()
{



	/*
	Quaternion addR;
	addR.SetRotationDegY(0.5);
	auto rot = m_test->GetRotation();
	rot.Multiply(addR);
	addR.SetRotationDegZ(0.6);
	rot.Multiply(addR);
	addR.SetRotationDegX(0.3);
	rot.Multiply(addR);
	m_test->SetRotation(rot);
	*/
}

void Game::PostUpdate()
{
	auto tarPos = m_player->GetPosition();
	m_camera->SetTarget(tarPos);
}

void Game::Render()
{
}

void Game::PostRender()
{
}
