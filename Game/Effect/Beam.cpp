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
	PlaneParticleEffectInitData pid;
	pid.m_ddsFilePath = L"Assets/Image/illumination.dds";
	pid.m_height = 10;
	pid.m_width = 10;
	pid.m_isBillboard = true;

	PlaneParticleUpdater m_effctUpdater(
		[&]PLANE_PARTICLE_GENERATE_FUNC(pThis, deltaTime)
	{
		//pThis->AddParticle(pos, g_vec3One * m_particleScale, g_quatIdentity, m_particleColor, lifeTime, nullptr, false);
	},
		[&]PLANE_PARTICLE_UPDATE_FUNC(data, deltaTime, extendData) {

	});
}



