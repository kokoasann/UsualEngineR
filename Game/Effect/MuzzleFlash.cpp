#include "stdafx.h"
#include "MuzzleFlash.h"


MuzzleFlash::MuzzleFlash()
{

}

MuzzleFlash::~MuzzleFlash()
{

}



void MuzzleFlash::Release()
{
	DeleteGO(m_effect);
}

void MuzzleFlash::OnDestroy()
{

}


void MuzzleFlash::Init(const MuzzleFlashEffectInitData& mfid)
{
	m_particleLifeTime = mfid.particleLifeTime;
	m_particleScale = mfid.particleScale;
	m_particleColor = mfid.particleColor;
	m_particleEndColor = mfid.particleEndColor;

	m_circleNum = mfid.circleNum;
	m_circleStrokeNum = mfid.circleStrokeNum;
	m_circleSize = mfid.circleSize;
	m_velocity = mfid.velocity;

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

			for (int i = 0; i < m_circleNum; i++)
			{
				Vector3 pos = { m_circleSize,0.f,0.f };
				Quaternion rot;
				rot.SetRotationY(rotRad * i);
				rot.Apply(pos);
				Vector3 velo = m_velocity;
				rot.Apply(velo);
				for (int j = 0; j < m_circleStrokeNum; j++)
				{
					pos += velo;
					
					float lifeTime = m_particleLifeTime * (stNum -j) / stNum;
					pThis->AddParticle(pos, g_vec3One * m_particleScale, g_quatIdentity, m_particleColor, lifeTime, (int*)nullptr, false);
				}
			}
			m_isPlay = false;
			pThis->SetGenerateFlag(false);
		}

	},
		[&]PLANE_PARTICLE_UPDATE_FUNC(data, deltaTime, extendData)
	{
		//data.particleData.mulColor.w = Math::Lerp(data.lifeTime/m_particleLifeTime, 0.f, 1.f);
		data.particleData.mulColor.Lerp(data.lifeTime / m_particleLifeTime, m_particleEndColor, m_particleColor);
	});
	pid.m_updater = &m_effctUpdater;
	m_effect = NewGO<PlaneParticleEffectRender>(0);
	m_effect->Init(pid);

	m_effect->SetGenerateFlag(false);
}


void MuzzleFlash::Awake()
{

}

bool MuzzleFlash::Start()
{
	return true;
}


void MuzzleFlash::PreUpdate()
{

}

void MuzzleFlash::Update()
{

}

void MuzzleFlash::PostUpdate()
{

}

void MuzzleFlash::Play()
{
	m_isPlay = true;
	m_effect->SetGenerateFlag(true);
}

void MuzzleFlash::Stop()
{
	m_isPlay = false;
	m_effect->SetGenerateFlag(false);
}



