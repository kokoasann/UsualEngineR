#include "stdafx.h"
#include "Ballistic.h"


Ballistic::Ballistic()
{

}

Ballistic::~Ballistic()
{

}



void Ballistic::Release()
{

}

void Ballistic::OnDestroy()
{

}

void Ballistic::Init()
{
	PlaneParticleEffectInitData pid;
	pid.m_ddsFilePath = L"Assets/Image/illumination.dds";
	pid.m_height = 10;
	pid.m_width = 10;
	//pid.m_extendDataSize = sizeof(ParticleData);
	pid.m_isBillboard = true;

	PlaneParticleUpdater m_effctUpdater(
		[&]PLANE_PARTICLE_GENERATE_FUNC(pThis, deltaTime)
	{
		{
			float rotRad = Math::PI2 / m_circleNum;
			float stNum = static_cast<float>(m_circleStrokeNum - 1);

			//for (int i = 0; i < m_circleNum; i++)
			{
				Vector3 pos = { m_circleSize,0.f,0.f };
				Quaternion rot;
				//rot.SetRotationY(rotRad * i);
				//rot.Apply(pos);
				//Vector3 velo = m_velocity;
				//rot.Apply(velo);
				for (int j = 0; j < m_circleStrokeNum; j++)
				{
					//pos += velo;

					float lifeTime = m_particleLifeTime * (stNum - j) / stNum;
					pThis->AddParticle({0,10.f * deltaTime * j,0}, g_vec3One * m_particleScale, g_quatIdentity, m_particleColor, lifeTime, nullptr);
				}
			}
		}

	},
		[&]PLANE_PARTICLE_UPDATE_FUNC(data, deltaTime, extendData)
	{
		//data.particleData.mulColor.w = Math::Lerp(data.lifeTime/m_particleLifeTime, 0.f, 1.f);
		data.particleData.mulColor.Lerp(data.lifeTime / m_particleLifeTime, m_particleEndColor, m_particleColor);
		//data.particleData.pos.y += 10.f * deltaTime;
	});
	pid.m_updater = &m_effctUpdater;
	m_effect = NewGO<PlaneParticleEffectRender>(0);
	m_effect->Init(pid);

	Stop();
}


void Ballistic::Awake()
{

}

bool Ballistic::Start()
{
	return true;
}


void Ballistic::PreUpdate()
{

}

void Ballistic::Update()
{

}

void Ballistic::PostUpdate()
{

}

void Ballistic::Play()
{
	m_isPlay = true;
	m_effect->SetGenerateFlag(true);
}

void Ballistic::Stop()
{
	m_isPlay = false;
	m_effect->SetGenerateFlag(false);
}



