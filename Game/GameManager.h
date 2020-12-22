#pragma once
class GameStage;
class Player;
class EnemyTest;
class GameCamera;
class GameHUD;
class ItemManager;
class GameSceneMenu;
class Game;
class Goal;
class Church;
class Fade;

class GameManager
{

public:
	static GameManager& GetInstance() {
		static GameManager gm;
		return gm;
	}

	void InitGameWorld();
	void Release();

	void SpawnPlayer();

	Player* GetPlayer() const{
		return m_player;
	}

	GameCamera* GetGameCamera() const {
		return m_camera;
	}

	Church* GetChurch()const {
		return m_church;
	}

	GameCamera* m_camera = nullptr;
	Player* m_player = nullptr;

	//Level
	GameStage* m_stage = nullptr;

	//HUD
	GameHUD* m_gameHUD = nullptr;

	//Menu
	GameSceneMenu* m_menu = nullptr;

	ItemManager*  m_itemManager = nullptr;

	//GameScene
	Game* m_gameScene = nullptr;;

	//Goal
	Goal* m_goal = nullptr;

private:
	Church* m_church = nullptr;
	Fade* m_fade = nullptr;
};
