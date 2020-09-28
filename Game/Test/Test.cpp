#include "stdafx.h"
#include "Test.h"
#include "ThreadTest.h"

void Test::Release()
{
	
}

void Test::OnDestroy()
{
}

void Test::Awake()
{
	{
		auto cam = g_lockCamera3D.Get();
		cam->SetPosition({ 10,0,0 });
		cam->SetTarget({ 0,0,0 });
	}

	/*m_threadObj.Execute([&]()
		{
			m_threadTest = NewGO<ThreadTest>(0);
		});*/
	
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test_criss.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_fxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";

	m_physicsModel = NewGO<ModelRender>(0);
	
	m_physicsModel->Init(mid);
	
	
	m_pso.CreateMeshObject(m_physicsModel->GetModel(),g_vec3Zero,g_quatIdentity,g_vec3One);
	

	mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";
	m_pModel = NewGO<ModelRender>(0);
	m_pModel->Init(mid);
	m_pModel->SetPosition({ 0,10,0 });

	m_meshColl.CreateFromSkinModel(m_pModel->GetModel());
	m_sphere.Create(0.5);
	RigidBodyInfo info;
	info.collider = &m_meshColl;
	info.mass = 1;
	info.pos = m_pModel->GetPosition();
	m_rb.Create(info);
	Physics().AddRigidBody(m_rb);
}

void Test::Update()
{
	m_pModel->SetPosition(m_rb.GetBody()->getWorldTransform().getOrigin());
}
