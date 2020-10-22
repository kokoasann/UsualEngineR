#include "stdafx.h"
#include "Test.h"
#include "ThreadTest.h"


#include "Effect/ParticleEffect.h"

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
		cam->SetPosition({ 0,20,10 });
		cam->SetTarget({ 0,0,0 });
	}

	/*m_threadObj.Execute([&]()
		{
			m_threadTest = NewGO<ThreadTest>(0);
		});*/
	
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test_criss.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";

	m_physicsModel = NewGO<ModelRender>(0);
	
	m_physicsModel->Init(mid);
	m_physicsModel->SetPosition({ 0,0,0 });
	
	m_pso.CreateMeshObject(m_physicsModel->GetModel(),m_physicsModel->GetPosition(),g_quatIdentity,g_vec3One);
	

	mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";
	m_pModel = NewGO<ModelRender>(0);
	m_pModel->Init(mid);
	m_pModel->SetPosition({ 0,500,0 });
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

	
	/*m_threads[0].Execute([&]()
		{
			Stopwatch sw;
			sw.Start();
			ModelInitData mid;
			mid.m_upAxis = EUpAxis::enUpAxisZ;
			mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
			mid.m_vsEntryPointFunc = "VSMain";
			mid.m_psEntryPointFunc = "PSMain";
			m_level.Init("Assets/level/map_level.tkl", [&](LevelObjectData& objData)->bool
				{
					std::string name(objData.name.begin(), objData.name.end());
					char filePath[256];
					sprintf_s(filePath, "Assets/modelData/map/%s.tkm", name.c_str());
					ModelRender* mr = NewGO<ModelRender>(0);
					mid.m_tkmFilePath = filePath;
					mr->Init(mid);
					m_mapmodel.push_back(mr);
					return true;
				});
			m_isEndLoad = true;
			m_loadTime = sw.Stop();
		});
	m_threads[1].Execute([&]()
		{
			Stopwatch sw;
			sw.Start();
			ModelInitData mid;
			mid.m_upAxis = EUpAxis::enUpAxisZ;
			mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
			mid.m_vsEntryPointFunc = "VSMain";
			mid.m_psEntryPointFunc = "PSMain";

			m_level.Init("Assets/level/map_physics_level.tkl", [&](LevelObjectData& objData)->bool
				{
					std::string name(objData.name.begin(), objData.name.end());
					char filePath[256];
					sprintf_s(filePath, "Assets/modelData/map_physics/%s.tkm", name.c_str());
					mid.m_tkmFilePath = filePath;

					Model model;
					model.Init(mid);
					m_mapPSOList[m_count].CreateMeshObject(model, objData.position, objData.rotation, objData.scale);
					m_count++;
					return true;
				});
			m_isEndLoad = true;
			m_loadTime = sw.Stop();
		});*/
	/*m_level.Init("Assets/level/map_level.tkl", [&](LevelObjectData& objData)->bool
		{
			std::string name(objData.name.begin(), objData.name.end());
			m_tkms.push_back(std::move(name));
			return true;
		});
	for (int i = 0; i < 10; i++)
	{
		m_threads[i].Execute([&,i]()
			{
				ModelInitData mid;
				mid.m_upAxis = EUpAxis::enUpAxisZ;
				mid.m_fxFilePath = "Assets/shader/NoAnimModel.fx";
				mid.m_vsEntryPointFunc = "VSMain";
				mid.m_psEntryPointFunc = "PSMain";

				for (int j = 0; j < 10; j++)
				{
					char filePath[256];
					sprintf_s(filePath, "Assets/modelData/map/%s.tkm", m_tkms[i*10+j].c_str());
					mid.m_tkmFilePath = filePath;
					ModelRender* mr = NewGO<ModelRender>(0);

					mr->Init(mid);
					std::lock_guard<std::mutex>lock (m_loadMutex);
					m_mapmodel.push_back(mr);
				}
			});
	}*/

	/*m_threads[0].Execute([&]()
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
		});*/

	m_chara = NewGO<ModelRender>(0);
	m_chara->Init(mid);
	m_chara->SetPosition({ 10,100,0 });
	m_characon.Init(1, 2, m_chara->GetPosition(), true);

	m_3dSprite = NewGO<SpriteRender>(0);
	SpriteInitData sd;
	sd.m_ddsFilePath[0] = "Assets/Image/sample.dds";
	sd.m_height = 50;
	sd.m_width = 50;
	m_3dSprite->Init(sd);
	m_3dSprite->MainCameraView();
	m_3dSprite->CameraScaleLock();
	m_3dSprite->Mode_BillBord();
	m_3dSprite->SetPos({ 50,10,0 });
	m_3dSprite->SetPivot({ 0,0 });
	m_3dSprite->SetSca(g_vec3One * 0.001f);

	m_3dSprite = NewGO<SpriteRender>(1);
	m_3dSprite->Init(sd);
	m_3dSprite->MainCameraView();
	//m_3dSprite->CameraScaleLock();
	m_3dSprite->Mode_BillBord();
	m_3dSprite->SetPos({ 50,10,0 });
	m_3dSprite->SetPivot({ 0,0 });

	m_3dSprite->SetSca(g_vec3One * 0.1f);


	PlaneParticleEffectInitData pid;
	pid.m_ddsFilePath = L"Assets/Image/hp.dds";
	pid.m_height = 10;
	pid.m_width = 10;
	PlaneParticleUpdater m_effctUpdater(
		[&](PLANE_PARTICLE_GENERATE_ARGS_CONST)->void
		{
			static float time = 0;
			if (time > 1.f)
			{
				Matrix m = g_matIdentity;
				pThis->AddParticle(m, { 1,1,1,1 }, 10);
				time = 0;
			}
			time += deltaTime;
		},
		[&](PLANE_PARTICLE_UPDATE_ARGS_CONST)->void
		{
			data.mWorld.v[3].y += 5.f * deltaTime;
			float n = GPerlinNoise().GenerateNoise({ 0,data.mWorld.v[3].y });
			data.mWorld.v[3].x = n*500.f* deltaTime;
		});
	pid.m_updater = &m_effctUpdater;
	auto effect = NewGO<PlaneParticleEffectRender>(0);
	effect->Init(pid);
}

void Test::Update()
{
	m_chara->SetPosition(m_characon.Execute(gameTime()->GetDeltaTime(), (Vector3&)(g_vec3AxisY * -10.f)));

	auto& trans = m_rb.GetBody()->getWorldTransform();
	m_pModel->SetPosition(trans.getOrigin());
	m_pModel->SetRotation(trans.getRotation());

	auto cam = g_lockCamera3D.Get();
	Quaternion rot;
	rot.SetRotationDegY(0.5f);
	auto pos = cam->GetPosition();
	rot.Apply(pos);
	cam->SetPosition(pos);
	static Vector3 p = { 50,10,0 };
	rot.Apply(p);
	//m_3dSprite->SetPos(p);
}

void Test::Render()
{
}
