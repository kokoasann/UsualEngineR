#include "stdafx.h"
#include "Title.h"
#include "Game.h"

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
	return true;
}


void Title::PreUpdate()
{

}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		NewGO<Game>(0);
		auto go = reinterpret_cast<GameObject*>(this);
		DeleteGO(go);
	}
}

void Title::PostUpdate()
{

}


void Title::Render()
{

}

void Title::PostRender()
{

}
