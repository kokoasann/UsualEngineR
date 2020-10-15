#pragma once
#include "../UsualEngineR/Graphics/SpriteRender.h"
class Player;
class IEnemy;

/// <summary>
/// 
/// </summary>
class GameHUD :public GameObject
{
public:
	GameHUD();
	virtual ~GameHUD();

	/// <summary>
	/// 本開放。確保したものを開放するための関数。
	/// </summary>
	virtual void Release() override;

	/// <summary>
	/// このオブジェクトの所持するゲームオブジェクト等を消すための関数。
	/// 面倒くさければRelease関数と同じでもよい。
	/// </summary>
	virtual void OnDestroy() override;

	/// <summary>
	/// NewGO時に即座に呼ばれる関数。
	/// コンストラクタの代わりに使おう。
	/// </summary>
	void Awake() override;

	/// <summary>
	/// スタート関数。
	/// </summary>
	/// <returns>trueを返さない限り、ずっとStartを呼び続ける</returns>
	bool Start() override;

	/// <summary>
	/// 更新。の前に呼ばれる更新。
	/// </summary>
	void PreUpdate() override;
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
	/// <summary>
	/// 更新。の後に呼ばれる更新。
	/// </summary>
	void PostUpdate() override;

	/// <summary>
	/// 描画。
	/// </summary>
	void Render() override;

	/// <summary>
	/// 手前に表示するやつの描画。
	/// </summary>
	void PostRender() override;

	void SetPlayer(Player* player) {
		m_pPlayer = player;
	}

private:

	//Ref
	Player* m_pPlayer = nullptr;
	//IEnemy* m_enemy = nullptr;

	//const
	const Vector3 ANCHOR_BOTTOM_LEFT = { -(FRAME_BUFFER_W / 2.f), -(FRAME_BUFFER_H / 2.f), 0.f };
	const Vector3 ANCHOR_TOP_CENTER = { 0.f, FRAME_BUFFER_H / 2, 0.f };
	const Vector2 m_SPRITE_PIVOT = { 1.f,0.f };

	//Player HP
	SpriteRender* m_spPlayerHP = nullptr;

	const float m_flSpHpHeight = 40;
	const float m_flSpHpWidth = 250;
	Vector3 m_playerHpScale = Vector3::One;

	Vector3 m_hpPos = { 
		ANCHOR_BOTTOM_LEFT.x + 100.f,
		ANCHOR_BOTTOM_LEFT.y + 100, 
		0.f 
	};

	//Player Endurance
	SpriteRender* m_spPlayerEndurance = nullptr;

	const float m_flSpEnduranceHeight = 40.f;
	const float m_flSpEnduranceWidth = 250.f;
	Vector3 m_playerEnduranceScale = Vector3::One;

	Vector3 m_endurancePos = {
		ANCHOR_BOTTOM_LEFT.x + 100.f,
		ANCHOR_BOTTOM_LEFT.y + 30.f,
		0.f
	};


	//Player Boost
	SpriteRender* m_spPlayerBoost = nullptr;

	const float m_flSpBoostHeight = 150.f;
	const float m_flSpBoostWidth = 70.f;
	const Vector2 m_boostSpPivot = { 1.f,0.f };
	Vector3 m_playerBoostScale = Vector3::One;

	Vector3 m_boostPos = {
		ANCHOR_BOTTOM_LEFT.x + 10.f,
		ANCHOR_BOTTOM_LEFT.y + 10.f,
		0.f
	};

	//Enemy HP
	SpriteRender* m_spEnemyHP = nullptr;
	const float m_flSpEnemyHPHeight = 20.f;
	const float m_flSpEnemyHPWidth = 800.f;
	Vector3 m_enemyHpScale = Vector3::One;

	Vector3 m_enemyHPPos = {
		ANCHOR_TOP_CENTER.x - m_flSpEnemyHPWidth / 2.f,
		ANCHOR_TOP_CENTER.y  - m_flSpEnemyHPHeight  - 20.f,
		0.f
	};

};
