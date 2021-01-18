#pragma once
#include "Physics/SphereCollider.h"
#include "Physics/BoxCollider.h"
#include "level/Level.h"
#include "Character/CharacterController.h"
#include "Effect/VolumetricEffect.h"
#include "Effect/ParticleEffect.h"
class ThreadTest;
//class VolumetricEffectRender;
class Test:public GameObject
{
public:
	Test() {}
	virtual void Release() override;
	virtual void OnDestroy() override;
	void Awake() override;
	void Update() override;
	void Render() override;

	/// <summary>
	/// translate,scale,rotateの行列を作ってワールド行列を作るやり方と
	/// 最適化?したワールド行列を作るやり方の
	/// 速度の検証
	/// 結論：
	/// translate,scale,rotateの行列の生成に時間がかかるのでこれらすべてを作る必要な場合は
	/// 最適化したものを使う方が速い。
	/// </summary>
	void Test_CreateTransform();
private:
	ThreadTest* m_threadTest = nullptr;
	ThreadObject m_threadObj;

	ModelRender* m_pModel;
	MeshCollider m_meshColl;
	SphereCollider m_sphere;
	BoxCollider m_box;
	RigidBody m_rb;
	

	ModelRender* m_physicsModel;
	PhysicsStaticObject m_pso;

	ThreadObject m_threads[11];
	std::vector<ModelRender*> m_models;
	Vector3 m_pos = g_vec3Zero;
	Lock<std::vector<ModelRender*>> m_lockModels = Lock<std::vector<ModelRender*>>(&m_models);

	std::vector<CAnimationClipPtr> m_animlist;

	ModelRender* m_chara;
	CharacterController m_characon;

	Level m_level;
	std::vector<ModelRender*> m_mapmodel;
	std::vector<std::string> m_tkms;
	Model m_mapPhysicsList[100];
	PhysicsStaticObject m_mapPSOList[100];
	std::mutex m_loadMutex;
	int m_count = 0;
	bool m_isEndLoad = false;
	float m_loadTime = 0.0f;

	SpriteRender* m_3dSprite;


	VolumetricEffectRender* m_vol_box;
	VolumetricEffectRender* m_vol_sphere;

	PlaneParticleEffectRender* m_effect;
	PlaneParticleEffectRender* m_effect_2;
	PlaneParticleEffectRender* m_effects[4];

	Bone* m_backpackBone[6] = {nullptr};

	ModelRender* m_playModel;

	RigidBody m_rigid_Test1;
	RigidBody m_rigid_Test2;
	BoxCollider m_boxTest1;
	BoxCollider m_boxText2;
};