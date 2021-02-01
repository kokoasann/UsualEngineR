#pragma once
#include "level/Level.h"
#include "Effect/VolumetricEffect.h"
/// <summary>
/// 
/// </summary>
class GameStage :public GameObject
{
public:
	GameStage();
	virtual ~GameStage();

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

	const bool HasMapLoadingDone() {
		return m_isLoaded;
	}

private:
	ThreadObject m_threadForLevel;
	Level m_level;
	Level m_phyLevel;
	Level m_footholdLevel;
	Level m_decoLevel;
	
	const float m_levelScale = 100;
	std::vector<ModelRender*> m_mapmodel;
	std::vector<std::string> m_tkms;

	ThreadObject m_threadForCreatingMeshCol;
	PhysicsStaticObject m_pso;
	int m_count = 0;
	PhysicsStaticObject m_mapPSOList[100];

	int m_mapObjPSOCount = 0;
	PhysicsStaticObject m_mapObjPSOList[42];

	bool m_isRegistRigidBody = false;			//剛体登録完了??
	std::vector<RigidBody*> m_rigidBodys;		//剛体達。

	bool m_isLoaded = false;

	std::vector<ModelRender*> m_mbgs;


	VolumetricEffectRender* m_sandSmoke = nullptr;
};
