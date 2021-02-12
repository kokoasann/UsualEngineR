#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"
#include "OpeningScene.h"

bool Title::m_seLoaded = false;

Title::Title()
{

}

Title::~Title()
{

}



void Title::Release()
{
	DeleteGO(m_backSprite);
}

void Title::OnDestroy()
{
	Release();
}


void Title::Awake()
{

}

bool Title::Start()
{
	if (!m_seLoaded) {
		m_seLoaded = true;
		LoadSoundSources();
		printf("load se\n");
	}

	Fade::GetInstance().FadeIn();

	SpriteInitData sd;
	sd.m_ddsFilePath[0] = "Assets/Image/title.dds";
	sd.m_width = 1280;
	sd.m_height = 720;
	m_backSprite = NewGO<SpriteRender>(0);
	m_backSprite->Init(sd);
	m_backSprite->SetPos(m_backSpPos);

	return true;
}


void Title::PreUpdate()
{
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA) and !m_isTrigeredStart) {
		Fade::GetInstance().FadeOut();
		m_isTrigeredStart = true;
	}

	if (Fade::GetInstance().IsFaded() and m_isTrigeredStart) {

		//NewGO<Game>(0);
		NewGO<OpeningScene>(0);
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


void Title::LoadSoundSources() {
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Bash.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Big_NockDown.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Boss_Shout.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Big_NockDown.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Encounter.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/esca.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Goblin_Punch1.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Goblin_StrongPunch1.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Goblin_Voice1.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/gogogo.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Heal.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Heal2.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/kick-low1.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/LongEnemy_Fire.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/LongEnemy_Target.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/mini_Explosion.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/omoi_panchi.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/shoot.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/TPSE.wav");
		DeleteGO(se);
	}

	//fatman
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/boss_fatman/backstep.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/boss_fatman/Balkan.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/boss_fatman/Balkan2.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/boss_fatman/charge.wav");
		DeleteGO(se);
	}
	//chara
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/beam.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/explosion.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/footstep1.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/footstep2.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/hugeExplosion.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/jetSe.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/mg.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/missile.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/punch_2_1.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/punch_2_2.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/punch1.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/punch2.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/punch3.wav");
		DeleteGO(se);
	}
	{
		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/shoot.wav");
		DeleteGO(se);
	}
}