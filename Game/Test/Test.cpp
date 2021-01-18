#include "stdafx.h"
#include "Test.h"
#include "ThreadTest.h"


#include "Effect/ExplosionEffect.h"


struct SweepResult : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;
	Vector3 hitPos = Vector3::Zero;
	const Vector3& startPos;
	float dist = FLT_MAX;
	int collAttr = enCollisionAttr_None;
	btCollisionObject* hitColl = nullptr;

	SweepResult(const Vector3& v) :
		startPos(v)
	{

	}

	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		int index = convexResult.m_hitCollisionObject->getUserIndex();
		//キャラコンでもグラウンドでもない場合早期出社。
		if (!(index & enCollisionAttr_Character
			|| index & enCollisionAttr_Ground))
		{
			//return 0.f;
		}

		isHit = true;
		Vector3 hitp = *(Vector3*)&convexResult.m_hitPointLocal;
		Vector3 div = startPos - hitp;
		float dis = div.Length();
		if (dis < dist)
		{
			hitPos = *(Vector3*)&convexResult.m_hitPointLocal;
			dist = dis;
			collAttr = index;
			hitColl = (btCollisionObject*)convexResult.m_hitCollisionObject;
		}
		return 0.0f;
	}
};

void Test::Release()
{
	Physics().RemoveRigidBody(m_rb);
}

void Test::OnDestroy()
{
}

void Test::Awake()
{
	CSoundSource* se = NewGO<CSoundSource>(0);
	se->Init(L"Assets/sound/kick-low1.wav");
	{
		auto cam = g_lockCamera3D.Get();
		cam->SetPosition({ 0,20,10 });
		cam->SetTarget({ 0,0,0 });
	}
	//se->Play(true);

	m_boxTest1.Create({ 10,10,10 });
	m_boxText2.Create({ 1,2.5,1 });
	RigidBodyInfo rbinfo;
	rbinfo.collider = &m_boxTest1;
	rbinfo.pos = { 0,30,0 };
	m_rigid_Test1.Create(rbinfo);
	Physics().AddRigidBody(m_rigid_Test1);
	rbinfo.collider = &m_boxText2;
	rbinfo.pos = { 0,60,0 };
	m_rigid_Test2.Create(rbinfo);
	Physics().AddRigidBody(m_rigid_Test2);

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
	pid.m_isBillboard = true;
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
					pThis->AddParticle(g_vec3Zero, g_vec3One, g_quatIdentity, { 3,2.f,0.3,1 }, 10, i);
				}
				time = 0;
			}
			time += deltaTime;
		},
		[&]PLANE_PARTICLE_UPDATE_FUNC(data,deltaTime,extendData)
		{
			auto s = *(float*)extendData;
			data.particleData.pos.y += 30.f * deltaTime;

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
	m_effect = NewGO<PlaneParticleEffectRender>(0);
	m_effect->Init(pid);
	m_effect->SetPos({ 0,0,50 });
	m_effect->SetSca(g_vec3One * 0.1);
	m_effect_2 = NewGO<PlaneParticleEffectRender>(0);
	m_effect_2->Init(pid);
	m_effect_2->SetPos({ 0,0,50 });
	m_effect_2->SetSca(g_vec3One * 0.1);
	for (int i = 0; i < 4; i++)
	{
		m_effects[i] = NewGO<PlaneParticleEffectRender>(0);
		m_effects[i]->Init(pid);
		m_effects[i]->SetSca(g_vec3One * 0.05);
	}

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
	mid.m_tkmFilePath = "Assets/modelData/m/Backpack/Backpack.tkm";
	mid.m_tksFilePath = "Assets/modelData/m/Backpack/Backpack.tks";
	//mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	//mid.m_psEntryPointFunc = "PSMain_nor";

	//mid.m_upAxis = enUpAxisZ;
	mm->Init(mid);
	mm->SetPosition({ 100,50,100 });
	mm->SetScale({ 5,5,5 });
	mm->SetMulColor({ 0.5, 0.5, 0.4, 1.f });

	m_animlist.resize(1);
	m_animlist[static_cast<int>(0)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(0)]->Load("Assets/modelData/m/anim/m_walk.tka");
	m_animlist[static_cast<int>(0)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(0)]->SetLoopFlag(true);

	//mm->InitAnimation(m_animlist, 1);
	//mm->Play(0);

	m_playModel = NewGO<ModelRender>(0);
	mid.m_tkmFilePath = "Assets/modelData/m/m_exBone.tkm";
	mid.m_tksFilePath = "Assets/modelData/m/m_exBone.tks";

	m_playModel->Init(mid);
	m_playModel->SetPosition({ -100,50,-100 });
	m_playModel->SetScale({ 5,5,5 });
	m_playModel->SetMulColor({ 0.5, 0.5, 0.4, 1.f });

	m_backpackBone[0] = m_playModel->GetModel().GetSkelton()->GetBone(m_playModel->GetModel().GetSkelton()->FindBoneID(L"Sole_R"));
	m_backpackBone[5] = mm->GetModel().GetSkelton()->GetBone(mm->GetModel().GetSkelton()->FindBoneID(L"Pack.002"));
	m_backpackBone[1] = mm->GetModel().GetSkelton()->GetBone(mm->GetModel().GetSkelton()->FindBoneID(L"Bombe_L.001"));
	m_backpackBone[2] = mm->GetModel().GetSkelton()->GetBone(mm->GetModel().GetSkelton()->FindBoneID(L"Bombe_L.002"));
	m_backpackBone[3] = mm->GetModel().GetSkelton()->GetBone(mm->GetModel().GetSkelton()->FindBoneID(L"Bombe_R.001"));
	m_backpackBone[4] = mm->GetModel().GetSkelton()->GetBone(mm->GetModel().GetSkelton()->FindBoneID(L"Bombe_R.002"));


	std::function<void()> func1 = [&]()
	{
		ExplosionEffect* ef = NewGO<ExplosionEffect>(0);
		ef->Init();
		ef->Play();
	};
	std::function<void()> func2 = [&]()
	{
	};
	DebugSwitchAddCheck(DebugSwitchNewSwitch('Q', (char)0, func1, func2));
	
}

void Test::Update()
{
	//m_chara->SetPosition(m_characon.Execute(gameTime()->GetDeltaTime(), (Vector3&)(g_vec3AxisY * -10.f)));
	btTransform from, to;
	from.setIdentity();
	to.setIdentity();
	from.setOrigin({ 0,60,0 });
	to.setOrigin({ 0,11,0 });
	SweepResult sr({0,60,0});
	Physics().ConvexSweepTest((const btConvexShape*)m_boxText2.GetBody(), from, to, sr);
	if (sr.isHit)
	{
		//printf("aa");
	}

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

	{
		const auto& mat = m_backpackBone[0]->GetWorldMatrix();
		m_effect->SetRot(mat.GetRotate());
		m_effect->SetPos(mat.GetTransrate());
	}
	{
		const auto& mat = m_backpackBone[5]->GetWorldMatrix();
		m_effect_2->SetRot(mat.GetRotate());
		m_effect_2->SetPos(mat.GetTransrate());
	}
	for (int i = 0;i<4;i++)
	{
		const auto& mat = m_backpackBone[i+1]->GetWorldMatrix();
		m_effects[i]->SetRot(mat.GetRotate());
		m_effects[i]->SetPos(mat.GetTransrate());
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


void Test::Test_CreateTransform()
{
	DebugPrint_NOMOTO("create world transform time measure");

	DebugPrint_NOMOTO("too create world transform element");
	/*
	Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz
	old: 0.003011
	new: 0.000259
	*/
	{
		Matrix w;
		Stopwatch sw;
		sw.Start();
		for (int i = 0; i < 100000; i++)
		{
			Matrix tra, sca, rot;
			tra.MakeTranslation({ 0,0,0 });
			sca.MakeScaling({ 1,1,1 });
			rot.MakeRotationX(180);
			w.Multiply(sca, rot);
			w.Multiply(w, tra);
		}
		DebugPrintValue(EDebugConsoloUser::NOMOTO, "old", sw.Stop());
		DebugPrintMatrix(EDebugConsoloUser::NOMOTO, w);


		sw.Start();
		for (int i = 0; i < 100000; i++)
		{
			Vector3 vpos = g_vec3Zero, vsca = g_vec3One;
			Quaternion qrot = g_quatIdentity;
			w.MakeTransform(vpos, vsca, qrot);
		}
		DebugPrintValue(EDebugConsoloUser::NOMOTO, "new", sw.Stop());
		DebugPrintMatrix(EDebugConsoloUser::NOMOTO, w);
	}
	DebugPrint_NOMOTO("");


	DebugPrint_NOMOTO("already created world transform element");
	/*
	DirectXMathのMultiplyがかなり最適化されていてめちゃくちゃ速い。
	なので、下のようにすでにそれぞれの行列が作られている場合はMultiplyを
	使った方が速い。
	Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz
	old: 0.000113
	new: 0.000257
	*/
	{
		Matrix tra, sca, rot;
		tra.MakeTranslation({ 0,0,0 });
		sca.MakeScaling({ 1,1,1 });
		rot.MakeRotationX(180);
		Matrix w;
		Stopwatch sw;
		sw.Start();
		for (int i = 0; i < 100000; i++)
		{
			
			w.Multiply(sca, rot);
			w.Multiply(w, tra);
		}
		DebugPrintValue(EDebugConsoloUser::NOMOTO, "old", sw.Stop());
		DebugPrintMatrix(EDebugConsoloUser::NOMOTO, w);

		Vector3 vpos = g_vec3Zero, vsca = g_vec3One;
		Quaternion qrot = g_quatIdentity;
		sw.Start();
		for (int i = 0; i < 100000; i++)
		{
			
			w.MakeTransform(vpos, vsca, qrot);
		}
		DebugPrintValue(EDebugConsoloUser::NOMOTO, "new", sw.Stop());
		DebugPrintMatrix(EDebugConsoloUser::NOMOTO, w);
	}
	DebugPrint_NOMOTO("");

	DebugPrint_NOMOTO("case 2 make");
	/*
	下の実行結果から、Matrixのコピーでも時間がかかっている。

	Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz
	old: 0.002874
	new: 0.000436
	*/
	{
		Matrix w;
		Stopwatch sw;
		sw.Start();
		for (int i = 0; i < 100000; i++)
		{
			Matrix tra, sca, rot;
			tra.MakeTranslation({ 0,0,0 });
			sca.MakeScaling({ 1,1,1 });
			rot.MakeRotationX(180);
			w.Multiply(sca, rot);
			w.Multiply(w, tra);
			w.Multiply(sca, rot);
			w.Multiply(w, tra);
		}
		DebugPrintValue(EDebugConsoloUser::NOMOTO, "old", sw.Stop());
		DebugPrintMatrix(EDebugConsoloUser::NOMOTO, w);

		
		sw.Start();
		for (int i = 0; i < 100000; i++)
		{
			Vector3 vpos = g_vec3Zero, vsca = g_vec3One;
			Quaternion qrot = g_quatIdentity;
			w.MakeTransform(vpos, vsca, qrot);
			w.MakeTransform(vpos, vsca, qrot);
		}
		DebugPrintValue(EDebugConsoloUser::NOMOTO, "new", sw.Stop());
		DebugPrintMatrix(EDebugConsoloUser::NOMOTO, w);
	}
	DebugPrint_NOMOTO("");
}
