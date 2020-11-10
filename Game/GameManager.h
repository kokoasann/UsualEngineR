#pragma once
class GameStage;
class Player;
class EnemyTest;
class GameCamera;
class GameHUD;
class ItemManager;

class GameManager
{

public:
	static GameManager& GetInstance() {
		static GameManager gm;
		return gm;
	}

	void InitGameWorld();
	void Release();

	GameCamera* m_camera = nullptr;
	Player* m_player = nullptr;

	//Level
	GameStage* m_stage = nullptr;

	//HUD
	GameHUD* m_gameHUD = nullptr;

	ItemManager*  m_itemManager = nullptr;
};