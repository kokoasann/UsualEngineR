#pragma once
#include "Navimesh/Navimesh.h"

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

	GameSceneMenu* GetMenu() const {
		return m_menu;
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

	//Navimesh
	Navimesh m_nvm;

	void AddDeadBossCount() {
		m_deadBossCount++;
	}

	void ResetDeadBossCount() {
		m_deadBossCount = 0;
	}

	const int GetDeadBossCount() {
		return m_deadBossCount;
	}

	void AppearGoal();

private:
	Church* m_church = nullptr;
	int m_deadBossCount = 0;
};
