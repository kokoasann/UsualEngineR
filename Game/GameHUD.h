#pragma once
#include "../UsualEngineR/Graphics/SpriteRender.h"
#include "Player/Player.h"
//class Player;
class IEnemy;
class TargetMarker;

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

	//void SetPlayer(Player* player) {
	//	m_pPlayer = player;
	//}

private:

	//Ref
	//Player* m_pPlayer = nullptr;
	//IEnemy* m_enemy = nullptr;

	//const
	const Vector3 ANCHOR_BOTTOM_LEFT = { -(FRAME_BUFFER_W / 2.f), -(FRAME_BUFFER_H / 2.f), 0.f };
	const Vector3 ANCHOR_TOP_CENTER = { 0.f, FRAME_BUFFER_H / 2, 0.f };
	const Vector2 m_SPRITE_PIVOT = { 1.f,0.f };
	
	const Vector4 m_BACK_COLOR = { 0.1,0.1 ,0.1 ,0.7 };

	//Player HP
	SpriteRender* m_spPlayerHP = nullptr;
	SpriteRender* m_spPlayerHP_Deco[2] = { nullptr };	//左右の飾り 0が左 1が右。
	SpriteRender* m_spPlayerHP_Back = nullptr;
	const Vector4 m_HP_COLOR = {
		235.f / 255.f,
		78.f / 255.f,
		78.f / 255.f,
		1.0f
	};
	const Vector4 m_HP_DECO_COLOR = {
		m_HP_COLOR.x * 0.6f,
		m_HP_COLOR.y * 0.6f,
		m_HP_COLOR.z * 0.6f,
		1.0f
	};

	const float m_flSpHpHeight = 15;
	const float m_flSpHpWidth = 300;
	Vector3 m_playerHpScale = Vector3::One;

	Vector3 m_hpPos = { 
		ANCHOR_BOTTOM_LEFT.x + 53,
		ANCHOR_BOTTOM_LEFT.y + 30, 
		0.f 
	};

	//Player Endurance
	SpriteRender* m_spPlayerEndurance = nullptr;
	SpriteRender* m_spPlayerEndurance_Deco[2] = { nullptr };
	SpriteRender* m_spPlayerEndurance_Back = nullptr;
	const Vector4 m_ENDURANCE_COLOR = { 
		89.f/255.f,
		213.f / 255.f,
		76.f / 255.f,
		1.f 
	};
	const Vector4 m_ENDURANCE_DECO_COLOR = {
		m_ENDURANCE_COLOR.x * 0.6f,
		m_ENDURANCE_COLOR.y * 0.6f,
		m_ENDURANCE_COLOR.z * 0.6f,
		1.f
	};

	const float m_flSpEnduranceHeight = 15.f;
	const float m_flSpEnduranceWidth = 300.f;
	Vector3 m_playerEnduranceScale = Vector3::One;

	Vector3 m_endurancePos = {
		ANCHOR_BOTTOM_LEFT.x + 53.f,
		ANCHOR_BOTTOM_LEFT.y + 10.f,
		0.f
	};


	//Player Boost
	SpriteRender* m_spPlayerBoost = nullptr;
	SpriteRender* m_spPlayerBoost_Deco[2] = { nullptr };
	SpriteRender* m_spPlayerBoost_Back = nullptr;

	const Vector4 m_BOOST_COLOR = { 
		226.f/255.f,
		173.f/255.f,
		56.f/255.f,
		1.f 
	};
	const Vector4 m_BOOST_DECO_COLOR = {
		m_BOOST_COLOR.x * 0.6f,
		m_BOOST_COLOR.y * 0.6f,
		m_BOOST_COLOR.z * 0.6f,
		1.f
	};

	const float m_flSpBoostHeight = 150.f;
	const float m_flSpBoostWidth = 30.f;
	const Vector2 m_boostSpPivot = { 1.f,0.f };
	Vector3 m_playerBoostScale = Vector3::One;

	Vector3 m_boostPos = {
		ANCHOR_BOTTOM_LEFT.x + 10.f,
		ANCHOR_BOTTOM_LEFT.y + 10.f + 7.f,
		0.f
	};

	//Enemy HP
	SpriteRender* m_spEnemyHP = nullptr;
	SpriteRender* m_spEnemyHP_Deco[2] = { nullptr };
	SpriteRender* m_spEnemyHP_Back = nullptr;
	const float m_flSpEnemyHPHeight = 20.f;
	const float m_flSpEnemyHPWidth = 800.f;
	Vector3 m_enemyHpScale = Vector3::One;

	Vector3 m_enemyHPPos = {
		ANCHOR_TOP_CENTER.x - m_flSpEnemyHPWidth / 2.f,
		ANCHOR_TOP_CENTER.y  - m_flSpEnemyHPHeight  - 20.f,
		0.f
	};

	//Player Weapon
	Vector2 m_PRESET_PIVOT = { 0.5,0.5 };
	SpriteRender* m_presetBack = nullptr;
	const float m_presetBackHeight = 128.f;
	const float m_presetBackWidth = 128.f;

	Vector3 m_presetPos = {
		-ANCHOR_BOTTOM_LEFT.x - 72.f,
		ANCHOR_BOTTOM_LEFT.y + 72.f,
		0.f
	};

	const float m_weaponSelectScale = 1.f;
	const float m_weaponScale = 0.5f;

	const float m_presetHeight = 50.f;
	const float m_presetWidth = 50.f;

	float m_presetOffsetSize = 40.f;
	SpriteRender* m_presetSp[TO_INT(Player::EnAttackPreset::enNumPreset)] = { nullptr };
	Vector3 m_presetOffset[TO_INT(Player::EnAttackPreset::enNumPreset)] =
	{
		{0.f,-m_presetOffsetSize,0.f},	//default
		{-m_presetOffsetSize,0.f,0.f},	//remote
		{0.f,m_presetOffsetSize,0.f},	//melee
		{m_presetOffsetSize,0.f,0.f}	//bomb
	};

	Player::EnAttackPreset m_lateFramePreset = Player::EnAttackPreset::enNumPreset;

	//Marker
	TargetMarker* m_targetMarker = nullptr;
};
