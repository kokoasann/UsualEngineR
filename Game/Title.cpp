#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"

Title::Title()
{

}

Title::~Title()
{

}



void Title::Release()
{

}

void Title::OnDestroy()
{

}


void Title::Awake()
{

}

bool Title::Start()
{
	Fade::GetInstance().FadeIn();
	return true;
}


void Title::PreUpdate()
{
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		Fade::GetInstance().FadeOut();
		m_isTrigeredStart = true;
	}

	if (Fade::GetInstance().IsFaded() and m_isTrigeredStart) {
		NewGO<Game>(0);
		auto go = reinterpret_cast<GameObject*>(this);
		DeleteGO(go);
	}

}

void Title::PostUpdate()
{
	Fade::GetInstance().Update();
}


void Title::Render()
{

}

void Title::PostRender()
{

}
