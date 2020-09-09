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
	g_camera3D->SetPosition({ 0,0,500 });
	ModelInitData mid;
	mid.m_fxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	mid.m_tksFilePath = "Assets/modelData/unityChan.tks";

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);

	m_animlist.resize(1);
	m_animlist[0] = std::make_unique<CAnimationClip>();
	m_animlist[0]->Load("Assets/anim/unityChan/run.tka");
	m_animlist[0]->BuildKeyFramesAndAnimationEvents();
	m_animlist[0]->SetLoopFlag(true);
	
	m_model->InitAnimation(m_animlist,1);
	m_model->Play(0);
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
