#include "stdafx.h"
#include "Test.h"
#include "ThreadTest.h"
#include "level/Level.h"

void Test::Release()
{
	Physics().RemoveRigidBody(m_rb);
}

void Test::OnDestroy()
{
}

void Test::Awake()
{
	{
		auto cam = g_lockCamera3D.Get();
		cam->SetPosition({ 0,0,50 });
		cam->SetTarget({ 0,0,0 });
	}

	m_threadObj.Execute([&]()
		{
			m_threadTest = NewGO<ThreadTest>(0);
		});
	
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test_criss.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_fxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";

	m_physicsModel = NewGO<ModelRender>(0);
	
	m_physicsModel->Init(mid);
	m_physicsModel->SetPosition({ 0,0,0 });
	
	m_pso.CreateMeshObject(m_physicsModel->GetModel(),m_physicsModel->GetPosition(),g_quatIdentity,g_vec3One);
	

	mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";
	m_pModel = NewGO<ModelRender>(0);
	m_pModel->Init(mid);
	m_pModel->SetPosition({ 0,10,0 });
	m_pModel->SetScale(g_vec3One);


	Matrix tra, sca, rot;
	tra.MakeTranslation(m_pModel->GetPosition());
	//tra = g_matIdentity;
	sca.MakeScaling(m_pModel->GetScale());
	rot.MakeRotationFromQuaternion(m_pModel->GetRotation());

	Matrix mat;
	mat.Multiply(sca, rot);
	mat.Multiply(mat, tra);

	m_meshColl.CreateFromSkinModel(m_pModel->GetModel(),&mat);
	m_sphere.Create(1);
	m_box.Create(m_pModel->GetScale() *2.f);
	RigidBodyInfo info;
	info.collider = &m_box;
	info.mass = 1;
	info.pos = m_pModel->GetPosition();
	
	m_rb.Create(info);
	Physics().AddRigidBody(m_rb);

	m_animlist.resize(1);
	m_animlist[0] = std::make_unique<CAnimationClip>();
	m_animlist[0]->Load("Assets/anim/unityChan/walk.tka");
	m_animlist[0]->BuildKeyFramesAndAnimationEvents();
	m_animlist[0]->SetLoopFlag(true);

	Level level;
	level.Init("Assets/level/map_level.tkl",[&](LevelObjectData& objData)->bool
		{
			objData.name;
			return false;
		});
	
	m_threads[0].Execute([&]()
		{
			ModelInitData mid;
			mid.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
			mid.m_tksFilePath = "Assets/modelData/unityChan.tks";
			mid.m_upAxis = EUpAxis::enUpAxisY;
			mid.m_fxFilePath = "Assets/shader/AnimModel.fx";
			mid.m_vsEntryPointFunc = "VSMain";
			mid.m_psEntryPointFunc = "PSMain";
			for (int i = 0; i < 50; i++)
			{
				auto m = NewGO<ModelRender>(0);
				m->Init(mid);
				m->SetPosition(m_pos);
				m->SetScale(g_vec3One * 0.05);
				m->InitAnimation(m_animlist, m_animlist.size());

				auto list = m_lockModels.Get();
				list->push_back(m);

				m_pos += Vector3{2, 0, 2};
			}
		});
}

void Test::Update()
{
	auto& trans = m_rb.GetBody()->getWorldTransform();
	m_pModel->SetPosition(trans.getOrigin());
	m_pModel->SetRotation(trans.getRotation());
}
