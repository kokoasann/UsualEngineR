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



void SmokeEffect::Init(const Vector4& col, const Vector4& colE, const float lifeSpanParam, const int density, const bool isWorld)
{
	struct ParticleData
	{
		//Vector3 pos;
		//Vector3 dir;
		float lifeTime;
	};

	PlaneParticleEffectInitData pid;
	pid.m_ddsFilePath = L"Assets/Image/illumination.dds";
	pid.m_height = 10;
	pid.m_width = 10;
	pid.m_extendDataSize = sizeof(ParticleData);
	pid.m_isBillboard = true;
	m_isWorld = isWorld;
	m_density = density;
	m_lifeSpanParam = lifeSpanParam;
	m_colS = col;
	m_colE = colE;


	PlaneParticleUpdater m_effctUpdater(
		[&]PLANE_PARTICLE_GENERATE_FUNC(pThis, deltaTime)
	{
		{
			{

				Vector3 posv = m_oldPos - pThis->GetWorldMatrix().GetTransrate();
				Quaternion rot = pThis->GetWorldMatrix().GetRotate();
				rot.Inverse(rot);

				for (int i = 0; i < m_density; i++)
				{
					Vector3 pos(GRandom().Rand() - 0.5, GRandom().Rand() - 0.5, GRandom().Rand() - 0.5);

					pos = posv * ((float)i / ((float)m_density - 1.f) * m_effectScaleInverse);
					pos.y += (float)i / ((float)m_density - 1.f) * 50.f * deltaTime;
					rot.Apply(pos);

					//pos *= 15.f * 2.f;
					//pos.y += GRandom().Rand() * 45.;

					ParticleData pd;
					//pd.pos = pos;
					//pd.dir = pos;
					//pd.dir.y += 40.f;
					//pd.dir.Normalize();
					pd.lifeTime = GRandom().Rand() * m_lifeSpanParam;
					//pd.lifeTime = lifeSpan;
					//pd.dir.x = GRandom().Rand() * 80.;
					pThis->AddParticle(pos, g_vec3One * (GRandom().Rand() * 4.), g_quatIdentity, m_colS, pd.lifeTime, pd, m_isWorld);
				}
			}
		}
		m_oldPos = pThis->GetWorldMatrix().GetTransrate();
		m_particleTimer += deltaTime;
	},
		[&]PLANE_PARTICLE_UPDATE_FUNC(data, deltaTime, extendData)
	{
		ParticleData* pd = reinterpret_cast<ParticleData*>(extendData);
		//float t = data.lifeTime / pd->lifeTime;
		float t = min(data.lifeTime / pd->lifeTime,1.f);
		data.particleData.pos.y += m_speed * deltaTime;

		t = (powf(1.f - t * 2.f, 4.f));

		data.particleData.mulColor.Lerp(t, m_colS, m_colE);
	});

	pid.m_updater = &m_effctUpdater;
	m_effect = NewGO<PlaneParticleEffectRender>(0);
	m_effect->Init(pid);

	m_effect->SetGenerateFlag(false);
}