#include "stdafx.h"
#include "Beam.h"


Beam::Beam()
{

}

Beam::~Beam()
{

}



void Beam::Release()
{

}

void Beam::OnDestroy()
{

}


void Beam::Awake()
{

}

bool Beam::Start()
{
	return true;
}


void Beam::PreUpdate()
{

}

void Beam::Update()
{

}

void Beam::PostUpdate()
{

}

void Beam::Init(const BeamEffectInitData& bid)
{
	m_particleScale = bid.particleScale;
	m_particleColor = bid.particleColor;
	m_particleLifeTime = bid.particleLifeTime;

	PlaneParticleEffectInitData pid;
	pid.m_ddsFilePath = L"Assets/Image/illumination.dds";
	pid.m_height = 10;
	pid.m_width = 10;
	pid.m_isBillboard = true;

	PlaneParticleUpdater m_effctUpdater(
		[&]PLANE_PARTICLE_GENERATE_FUNC(pThis, deltaTime)
	{
		Vector3 pos = { 0.f,0.f,0.f };
		Vector3 moveSpeed = Vector3::Zero;
		const float speed = 5.f;
		moveSpeed = m_direction * speed;
		
		for (int i = 0; i < 100; i++) {
			pos += moveSpeed;
			pThis->AddParticle(pos, g_vec3One * m_particleScale, g_quatIdentity, m_particleColor, m_particleLifeTime, nullptr, false);
		}
		
		m_isPlay = false;
		pThis->SetGenerateFlag(false);
	},
		[&]PLANE_PARTICLE_UPDATE_FUNC(data, deltaTime, extendData) {
		
		//data.particleData.mulColor.Lerp(data.lifeTime/m_particleLifeTime, m_particleColor, m_particleColor);
	});
	pid.m_updater = &m_effctUpdater;
	m_effect = NewGO<PlaneParticleEffectRender>(0);
	m_effect->Init(pid);

	m_effect->SetGenerateFlag(false);
}

void Beam::Play()
{
	m_isPlay = true;
	m_effect->SetGenerateFlag(true);
}

void Beam::Stop()
{
	m_isPlay = false;
	m_effect->SetGenerateFlag(false);
}

