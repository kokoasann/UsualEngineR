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



void SmokeEffect::Init() {
	enum class ParticleKind
	{
		Big,
		Piece,
		Rise,
		Gass,
	};
	struct ParticleData
	{
		Vector3 pos;
		Vector3 dir;
		float lifeTime;
		ParticleKind kind;
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
			Vector4 col = { 0.f,0.f,0.f,0.5f };
			//float rotRad = Math::PI2 / m_circleNum;
			//float stNum = static_cast<float>(m_circleStrokeNum - 1);

			//if (m_particleTimer > 0.2f && m_particleTimer < 1.3f)
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
					pd.kind = ParticleKind::Gass;
					pd.dir.x = GRandom().Rand() * 80.;
					pThis->AddParticle(pos, g_vec3One * (GRandom().Rand() * 4.), g_quatIdentity, col, pd.lifeTime, pd, true);
				}
			}
			//m_isPlay = false;
			//pThis->SetGenerateFlag(false);
		}
		m_particleTimer += deltaTime;
	},
		[&]PLANE_PARTICLE_UPDATE_FUNC(data, deltaTime, extendData)
	{

		Vector4 col = { 1.f,1.f,1.f,0.1f };
		Vector4 colE = { 1.f,1.f,1.f,0.1f };

		ParticleData* pd = reinterpret_cast<ParticleData*>(extendData);
		float t = data.lifeTime / pd->lifeTime;
		switch (pd->kind)
		{
		case ParticleKind::Big:
		{
			data.particleData.pos = pd->pos + pd->dir * (60 * (1.f - powf(1.f - (1.f - t), 16.f)));

			data.particleData.mulColor.Lerp(t, colE, col);
		}
		break;
		case ParticleKind::Piece:
		{
			auto v = pd->dir * (t - 0.5f * 2.f * (t * t)) * 50.;
			data.particleData.pos += v;
			data.particleData.pos.y = pd->pos.y + v.y;

			data.particleData.mulColor.Lerp(t, colE, col);
		}
		break;
		case ParticleKind::Rise:
		{
			data.particleData.pos.y += 3.f;
			data.particleData.pos.y = pd->pos.y + (pd->dir.x * (1.f - powf(1.f - (1.f - t), 10.f)));

			data.particleData.mulColor.Lerp(t, colE, col);
		}
		break;
		case ParticleKind::Gass:
		{
			data.particleData.pos.y += 50.f * deltaTime;

			t = 1.f - (powf(1.f - t * 2.f, 4.f));

			data.particleData.mulColor.Lerp(t, col, col);
		}
		break;
		}
	});
	pid.m_updater = &m_effctUpdater;
	m_effect = NewGO<PlaneParticleEffectRender>(0);
	m_effect->Init(pid);

	m_effect->SetGenerateFlag(false);
}