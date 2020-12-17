#include "stdafx.h"
#include "SmokeEffect.h"


SmokeEffect::SmokeEffect()
{

}

SmokeEffect::~SmokeEffect()
{

}

void SmokeEffect::Release()
{
	DeleteGO(m_effect);
}

void SmokeEffect::OnDestroy()
{
	Release();
}


void SmokeEffect::Awake()
{

}

bool SmokeEffect::Start()
{
	return true;
}


void SmokeEffect::PreUpdate()
{

}

void SmokeEffect::Update()
{

}

void SmokeEffect::PostUpdate()
{

}



void SmokeEffect::Init(const Vector4& col, const Vector4& colE)
{
	struct ParticleData
	{
		Vector3 pos;
		Vector3 dir;
		float lifeTime;
	};

	PlaneParticleEffectInitData pid;
	pid.m_ddsFilePath = L"Assets/Image/illumination.dds";
	pid.m_height = 10;
	pid.m_width = 10;
	pid.m_extendDataSize = sizeof(ParticleData);
	pid.m_isBillboard = true;

	PlaneParticleUpdater m_effctUpdater(
		[&]PLANE_PARTICLE_GENERATE_FUNC(pThis, deltaTime)
	{
		{
			{
				for (int i = 0; i < 7; i++)
				{
					Vector3 pos(GRandom().Rand() - 0.5, GRandom().Rand() - 0.5, GRandom().Rand() - 0.5);
					pos *= 15.f * 2.f;
					pos.y += GRandom().Rand() * 45.;
					ParticleData pd;
					pd.pos = pos;
					pd.dir = pos;
					pd.dir.y += 40.f;
					pd.dir.Normalize();
					pd.lifeTime = GRandom().Rand() * 1.;
					pd.dir.x = GRandom().Rand() * 80.;
					pThis->AddParticle(pos, g_vec3One * (GRandom().Rand() * 4.), g_quatIdentity, col, pd.lifeTime, pd, true);
				}
			}
		}
		m_particleTimer += deltaTime;
	},
		[&]PLANE_PARTICLE_UPDATE_FUNC(data, deltaTime, extendData)
	{
		ParticleData* pd = reinterpret_cast<ParticleData*>(extendData);
		float t = data.lifeTime / pd->lifeTime;
		data.particleData.pos.y += 50.f * deltaTime;

		t = 1.f - (powf(1.f - t * 2.f, 4.f));

		data.particleData.mulColor.Lerp(t, col, colE);
	});

	pid.m_updater = &m_effctUpdater;
	m_effect = NewGO<PlaneParticleEffectRender>(0);
	m_effect->Init(pid);

	m_effect->SetGenerateFlag(false);
}