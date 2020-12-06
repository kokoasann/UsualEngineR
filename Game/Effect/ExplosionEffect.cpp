#include "stdafx.h"
#include "ExplosionEffect.h"


ExplosionEffect::ExplosionEffect()
{

}

ExplosionEffect::~ExplosionEffect()
{

}



void ExplosionEffect::Release()
{

}

void ExplosionEffect::OnDestroy()
{

}

void ExplosionEffect::Init()
{
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
			Vector4 col = { 3.f,1.f,0.3f,1.f };
			//float rotRad = Math::PI2 / m_circleNum;
			//float stNum = static_cast<float>(m_circleStrokeNum - 1);
			if (m_particleTimer < 0.6f)
			{
				for (int i = 0; i < 10; i++)
				{
					Vector3 pos(GRandom().Rand() - 0.5, GRandom().Rand() - 0.5, GRandom().Rand() - 0.5);
					pos *= 0.f * 2.f;
					ParticleData pd;
					pd.dir = pos;

					pd.dir.Normalize();
					pd.lifeTime = GRandom().Rand() * 3.;
					pd.kind = ParticleKind::Big;
					pd.pos = pos;

					pThis->AddParticle(pos, g_vec3One * (max(GRandom().Rand(), 0.5) * 5.), g_quatIdentity, col, pd.lifeTime, pd, false);
				}
			}
			if (m_particleTimer < 0.2f)
			{
				for (int i = 0; i < 20; i++)
				{
					Vector3 pos(GRandom().Rand() - 0.5, GRandom().Rand() - 0.5, GRandom().Rand() - 0.5);
					pos *= 20.f * 2.f;

					ParticleData pd;
					pd.pos = pos;
					pd.dir = pos;
					pd.dir.y += 20.f;
					pd.dir.Normalize();
					pd.lifeTime = GRandom().Rand() * 1.;
					pd.kind = ParticleKind::Piece;
					pThis->AddParticle(pos, g_vec3One * (GRandom().Rand() * 0.5), g_quatIdentity, col, pd.lifeTime, pd, false);
				}
			}
			
			if (m_particleTimer > 0.1f && m_particleTimer < 0.5f)
			{
				for (int i = 0; i < 10; i++)
				{
					Vector3 pos(GRandom().Rand() - 0.5, GRandom().Rand() - 0.5, GRandom().Rand() - 0.5);
					pos *= 20.f * 2.f;

					ParticleData pd;
					pd.pos = pos;
					pd.dir = pos;
					pd.dir.y += 0.f;
					pd.dir.Normalize();
					pd.lifeTime = GRandom().Rand() * 1.5;
					pd.kind = ParticleKind::Rise;
					pd.dir.x = GRandom().Rand() * 80.;
					pThis->AddParticle(pos, g_vec3One * (GRandom().Rand() * 3.), g_quatIdentity, col, pd.lifeTime, pd, false);
				}
			}

			if (m_particleTimer > 0.2f && m_particleTimer < 1.3f)
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
					pThis->AddParticle(pos, g_vec3One * (GRandom().Rand() * 4.), g_quatIdentity, col, pd.lifeTime, pd, false);
				}
			}
			//m_isPlay = false;
			//pThis->SetGenerateFlag(false);
		}
		m_particleTimer += deltaTime;
	},
		[&]PLANE_PARTICLE_UPDATE_FUNC(data, deltaTime, extendData)
	{
		Vector4 col = { 3.f,1.f,0.3f,1.f };
		Vector4 colE = { 3.f,1.f,0.3f,0.f };
		ParticleData* pd = reinterpret_cast<ParticleData*>(extendData);
		float t = data.lifeTime / pd->lifeTime;
		switch (pd->kind)
		{
		case ParticleKind::Big:
		{
			data.particleData.pos = pd->pos + pd->dir * (60 * (1.f-powf(1.f-(1.f-t),16.f)));

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
			
			data.particleData.mulColor.Lerp(t, { 0.04, 0.04 ,0.04 ,0. }, { 0.04, 0.04 ,0.04 ,0.8 });
		}
			break;
		}
	});
	pid.m_updater = &m_effctUpdater;
	m_effect = NewGO<PlaneParticleEffectRender>(0);
	m_effect->Init(pid);

	m_effect->SetGenerateFlag(false);

	//m_volumeEffect = NewGO<VolumetricEffectRender>(0);
	////volEff->Init("Assets/modelData/test/sphere.tkm");
	//m_volumeEffect->Init("Assets/modelData/test/block.tkm");
	//m_volumeEffect->SetPos({ 0,70,0 });
	//m_volumeEffect->SetSca({ 50,90,50 });
	//auto& fd = m_volumeEffect->GetFogData();
	//fd.concentration = 0.;
	//fd.decayCenterToXZ = 0.02f;
	//fd.decayCenterToY = 1.f;
	//fd.ratioParlinWorley = 0.6f;
	//fd.ratioParlin = 0.2f;
	//fd.color = m_volumeColor;
	//fd.centerOffsetY = -40;
}


void ExplosionEffect::Awake()
{

}

bool ExplosionEffect::Start()
{
	return true;
}


void ExplosionEffect::PreUpdate()
{

}

void ExplosionEffect::Update()
{
	/*auto& fogData = m_volumeEffect->GetFogData();
	fogData.offset.y -= 5.f * gameTime()->GetDeltaTime();
	if (!m_isTimeStop)
	{
		if (m_isPlay)
		{
			auto& fd = m_volumeEffect->GetFogData();
			fd.concentration = Math::Lerp(m_timer / m_timeLimit, 0.f, m_volumeConcentration);
		}
		else
		{
			auto& fd = m_volumeEffect->GetFogData();
			fd.concentration = Math::Lerp(m_timer / m_timeLimit, m_volumeConcentration, 0.f);
		}
		if (m_timeLimit <= m_timer)
		{
			m_isTimeStop = true;
			m_timer = 0.f;
		}
		else
			m_timer += gameTime()->GetDeltaTime();
	}*/
}

void ExplosionEffect::PostUpdate()
{

}



