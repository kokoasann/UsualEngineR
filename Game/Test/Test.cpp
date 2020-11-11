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

	//m_3dSprite = NewGO<SpriteRender>(0);
	//SpriteInitData sd;
	//sd.m_ddsFilePath[0] = "Assets/Image/sample.dds";
	//sd.m_height = 50;
	//sd.m_width = 50;
	//m_3dSprite->Init(sd);
	//m_3dSprite->MainCameraView();
	//m_3dSprite->CameraScaleLock();
	//m_3dSprite->Mode_BillBord();
	//m_3dSprite->SetPos({ 50,10,0 });
	//m_3dSprite->SetPivot({ 0,0 });
	//m_3dSprite->SetSca(g_vec3One * 0.001f);

	//m_3dSprite = NewGO<SpriteRender>(1);
	//m_3dSprite->Init(sd);
	//m_3dSprite->MainCameraView();
	////m_3dSprite->CameraScaleLock();
	//m_3dSprite->Mode_BillBord();
	//m_3dSprite->SetPos({ 50,10,0 });
	//m_3dSprite->SetPivot({ 0,0 });

	//m_3dSprite->SetSca(g_vec3One * 0.1f);


	PlaneParticleEffectInitData pid;
	pid.m_ddsFilePath = L"Assets/Image/illumination.dds";
	pid.m_height = 10;
	pid.m_width = 10;
	pid.m_extendDataSize = sizeof(float);
	//pid.m_isBillboard = false;
	//pid.m_isDepthTest = false;
	PlaneParticleUpdater m_effctUpdater(
		[&]PLANE_PARTICLE_GENERATE_FUNC(pThis,deltaTime)
		{
			static float time = 0;
			if (time >= 0.01f)
			{
				//Matrix m = g_matIdentity;
				//pThis->AddParticle(m, { 1,1,1,1 }, 10);
				for (int _i = 0; _i <3; _i++)
				{
					float i = GRandom().Rand();
					pThis->AddParticle(g_vec3Zero, g_vec3One, g_quatIdentity, { 3,2.f,0.3,1 }, 10, &i);
				}
				time = 0;
			}
			time += deltaTime;
		},
		[&]PLANE_PARTICLE_UPDATE_FUNC(data,deltaTime,extendData)
		{
			auto s = *(float*)extendData;
			data.particleData.pos.y += 10.f * deltaTime;

			float n = GPerlinNoise2D().GenerateNoise({ s*10, data.particleData.pos.y / 10.f });
			float m = GPerlinNoise2D().GenerateNoise({ data.particleData.pos.y / 10.f, s*10 });
			data.particleData.pos.x = n * 500.f * deltaTime;
			data.particleData.pos.z = m * 500.f * deltaTime;
			data.particleData.sca = g_vec3One * min((data.lifeTime / 10.f)+0.1f,1.f);

			Vector3 col;
			col.Lerp(data.lifeTime / 10.f, { 3,0.1f,0.0 }, { 3,1.5f,0.3 });
			data.particleData.mulColor.Set(col);
			data.particleData.mulColor.a = data.lifeTime / 10.f;
			
		});
	pid.m_updater = &m_effctUpdater;
	auto effect = NewGO<PlaneParticleEffectRender>(0);
	effect->Init(pid);
	effect->SetPos({ 0,0,50 });
	Quaternion effrot;
	effrot.SetRotationDegZ(30);
	//effect->SetRot(effrot);

	m_vol_box = NewGO<VolumetricEffectRender>(0);
	//volEff->Init("Assets/modelData/test/sphere.tkm");
	m_vol_box->Init("Assets/modelData/test/test.tkm");
	m_vol_box->SetPos({ 80,70,50 });
	m_vol_box->SetSca({ 50,50,50 });
	{
		auto& fd = m_vol_box->GetFogData();
		fd.concentration = 0.05;
		fd.decayCenterToXZ = 0.02f;
		fd.decayCenterToY = 0.02f;
		fd.ratioParlinWorley = 1.f;
	}
	m_vol_sphere = NewGO<VolumetricEffectRender>(0);
	m_vol_sphere->Init("Assets/modelData/test/sphere.tkm");
	//volEff->Init("Assets/modelData/test/test.tkm");
	m_vol_sphere->SetPos({ -80,50,-50 });
	m_vol_sphere->SetSca({ 50,50,50 });
	{
		auto& fd = m_vol_sphere->GetFogData();
		fd.ratioParlinWorley = 1.f;
	}

	auto vol_test = NewGO<VolumetricEffectRender>(0);
	vol_test->Init("Assets/modelData/test/block.tkm");
	vol_test->SetPos({ 190,70,50 });
	vol_test->SetSca({ 50,50,50 });
	{
		auto& tfb = vol_test->GetFogData();
		tfb.ratioParlinWorley = 0.8;
		
	}
	

	auto mm = NewGO<ModelRender>(0);
	mid.m_tkmFilePath = "Assets/modelData/m/m_ExBone.tkm";
	mid.m_tksFilePath = "Assets/modelData/m/m_ExBone.tks";
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	//mid.m_psEntryPointFunc = "PSMain_nor";

	//mid.m_upAxis = enUpAxisZ;
	mm->Init(mid);
	mm->SetPosition({ 100,10,100 });
	mm->SetScale({ 5,5,5 });
	mm->SetMulColor({ 0.5, 0.5, 0.4, 1.f });

	m_animlist.resize(1);
	m_animlist[static_cast<int>(0)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(0)]->Load("Assets/modelData/m/anim/m_walk.tka");
	m_animlist[static_cast<int>(0)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(0)]->SetLoopFlag(true);

	mm->InitAnimation(m_animlist, 1);
	mm->Play(0);

}

void Test::Update()
{
	//m_chara->SetPosition(m_characon.Execute(gameTime()->GetDeltaTime(), (Vector3&)(g_vec3AxisY * -10.f)));

	auto& trans = m_rb.GetBody()->getWorldTransform();
	m_pModel->SetPosition(trans.getOrigin());
	m_pModel->SetRotation(trans.getRotation());

	{
		auto& fogData = m_vol_sphere->GetFogData();
		fogData.offset.y -= 5.f * gameTime()->GetDeltaTime();
	}
	{
		auto& fogData = m_vol_box->GetFogData();
		fogData.offset.y -= 5.f * gameTime()->GetDeltaTime();
	}
	/*auto cam = g_lockCamera3D.Get();
	Quaternion rot;
	rot.SetRotationDegY(0.5f);
	auto pos = cam->GetPosition();
	rot.Apply(pos);
	cam->SetPosition(pos);
	static Vector3 p = { 50,10,0 };
	rot.Apply(p);*/
	//m_3dSprite->SetPos(p);
}

void Test::Render()
{
}
