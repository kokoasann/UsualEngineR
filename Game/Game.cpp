#include "stdafx.h"
#include "Game.h"

void Game::Release()
{
}

void Game::OnDestroy()
{
}

void Game::Awake()
{
	g_camera3D->SetPosition({ 0,0,10 });
	g_camera3D->SetTarget({ 0,0,0 });
	ModelInitData mid;
	mid.m_fxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	mid.m_tksFilePath = "Assets/modelData/unityChan.tks";

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
	m_test = NewGO<ModelRender>(0);
	m_test->Init(mid);
	m_test->SetPosition({ 0,0,0 });
	m_test->SetScale(g_vec3One);
}

bool Game::Start()
{
	return true;
}

void Game::PreUpdate()
{
}

void Game::Update()
{
	Quaternion addR;
	addR.SetRotationDegY(0.5);
	auto rot = m_test->GetRotation();
	rot.Multiply(addR);
	addR.SetRotationDegZ(0.6);
	rot.Multiply(addR);
	addR.SetRotationDegX(0.3);
	rot.Multiply(addR);
	m_test->SetRotation(rot);
}

void Game::PostUpdate()
{
}

void Game::Render()
{
}

void Game::PostRender()
{
}
