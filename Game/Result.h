#pragma once
#include "level/Level.h"

/// <summary>
/// 
/// </summary>
class Result :public GameObject
{
public:
	Result();
	virtual ~Result();

	struct SAttachmentPercentage {
		float defaultAttachment;
		float meleeAttachment;
		float remoteAttachment;
		float bombAttachment;
	};

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

	void Init(const double clearTime, const int eliminateCount, const SAttachmentPercentage data);

private:

	float m_clearTime = 0.f;
	std::vector<ModelRender*> m_robots;
	const Vector3 m_scale = { 1.5f,1.5f,1.5f };
	int m_numRobot = 50;

	ModelRender* m_ground = nullptr;

	const Vector3 m_resultCameraPos = Vector3(0.f, 9.f, 8.2f);
	const Vector3 m_resultCameraTarget = Vector3(0.f, 5.f, 0.f);

	//Back Sprite
	SpriteRender* m_backSprite = nullptr;
	Vector3 m_backSpPos = Vector3(0.f, 220.f, 0.f);
	//Vector3 m_backSpScale = Vector3(5.5f, 2.f, 1.f);
	Vector3 m_backSpScale = Vector3(1.f, 1.f, 1.f);

	Level m_level;
	const float m_levelScale = 1.5f;

	bool m_isTriggeredButtonA = false;

	double m_clearSec = 0.f;
	int m_corpseCount = 0;

	SAttachmentPercentage m_attachmentPercentageData;

	int m_displayRoboCount = 0;
	int m_robotCount = 0;


	//FONT
	enum class FONT_TYPE {
		TIME = 0,
		POW = 2,
		CORPSE = 4,
		DEF = 6,
		MELEE = 8,
		REMOTE = 10,
		BOMB = 12
	};

	static const int m_NUM_FONTS = 14;
	Font m_fonts[m_NUM_FONTS];
	Vector2 m_fontsPos[m_NUM_FONTS];
	std::wstring m_texts[m_NUM_FONTS];

	const Vector4 m_FontColor = Vector4::White;
	const float m_fontRot = 0.f;
	const float m_leftFontSca = 1.f;
	const float m_rightFontSca = 0.8f;

	const float X1 = -500.f;
	const float X2 = -300.f;
	const float X3 =	100.f;
	const float X4 =	400.f;

	const float Y1 = 300.f;
	const float LeftYSpace = 40.f;
	const float RightYSpace =30.f;

	SpriteRender* m_attachBack[4] = {nullptr};
	Vector4 m_attachColor[4] = {
		{ 0.5,0.5 ,0.5,1.f },
		{ 0.8,0. ,0.,1.f },
		{ 0.,0.8 ,0.,1.f },
		{ 0.,0. ,0.8,1.f }
	};
};
