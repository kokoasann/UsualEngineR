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
	m_chargeColor = bid.chargeColor;
	m_beamColor = bid.beamColor;
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

		if (m_isCharge) {
			//チャージ。
			for (int i = 0; i < 10; i++) {
				//プレイヤーの横方向に300.f分ずらす。
				pos = m_holizontalDir * 300.f;
				//横の移動量の決定。
				const float maxMoveY = 20.f;
				auto rand = GRandom().Rand();
				float add = maxMoveY * rand;
				Vector3 moveAdd = m_holizontalDir * add;
				pos += moveAdd;

				const float maxAngle = 360.f;
				auto rand2 = GRandom().Rand();
				Quaternion rot;
				rot.SetRotationDeg(m_direction, maxAngle * rand2);
				rot.Apply(pos);

				pThis->AddParticle(pos, g_vec3One * m_particleScale, g_quatIdentity, m_chargeColor, m_particleLifeTime, nullptr, false);
			}
		}
		else {
			//ビーム。
			const float beamSpeed = 5.f;
			moveSpeed = m_direction * beamSpeed;
			for (int i = 0; i < 10000; i++) {
				pos += moveSpeed;
				pThis->AddParticle(pos, g_vec3One * m_particleScale, g_quatIdentity, m_beamColor, m_particleLifeTime, nullptr, false);
			}
		}
		if (!m_isLoop) {
			m_isPlay = false;
			pThis->SetGenerateFlag(false);
		}
	},
		[&]PLANE_PARTICLE_UPDATE_FUNC(data, deltaTime, extendData) {
		if (m_isCharge) {
			//向きを反対にする。
			Vector3 dir = data.particleData.pos * -1;			
			//正規化。
			dir.Normalize();
			//動きを反映。
			Vector3 moveSpeed = Vector3::Zero;
			float chargeDistance = 30.f;
			moveSpeed = dir * chargeDistance;
			data.particleData.pos += moveSpeed;
		}
	});
	pid.m_updater = &m_effctUpdater;
	m_effect = NewGO<PlaneParticleEffectRender>(0);
	m_effect->Init(pid);

	if (!m_isLoop) {
		m_effect->SetGenerateFlag(false);
	}
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

