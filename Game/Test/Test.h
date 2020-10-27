#pragma once
#include "Physics/SphereCollider.h"
#include "Physics/BoxCollider.h"
#include "level/Level.h"
#include "Character/CharacterController.h"
#include "Effect/VolumetricEffect.h"
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
};