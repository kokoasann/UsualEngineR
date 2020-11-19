#include "stdafx.h"
#include "JetEffect.h"


JetEffect::JetEffect()
{

}

JetEffect::~JetEffect()
{

}



void JetEffect::Release()
{
	DeleteGO(m_particleEffect);
}

void JetEffect::OnDestroy()
{
	Release();
}


void JetEffect::Awake()
{

}

bool JetEffect::Start()
{
	return true;
}


void JetEffect::PreUpdate()
{

}

void JetEffect::Update()
{

}

void JetEffect::PostUpdate()
{
	m_particleEffect->SetPos(m_pos);
	m_particleEffect->SetRot(m_rot);
	m_particleEffect->SetSca(Vector3::One * EFFECT_SCALE);
}


void JetEffect::Render()
{

}

void JetEffect::PostRender()
{

}


void JetEffect::Init(const JetEffectInitParam& initParam) {

	EFFECT_SCALE = initParam.effectScale;
	EFFECT_SCALE_INV = initParam.effectScale_inv;
	PARTICLE_SCALE = initParam.particleScale;
	PARTICLE_LIFE_TIME = initParam.particleLifeTime;
	PARTICLE_Y_UP = initParam.particleYUp;

	//パーティクルの拡張構造体。
	struct ParticleData
	{
		float rnd;		//パーリンノイズで使うためのseed値的なもの
		Vector3 pos;	//パーティクルの初期位置(ローカル)
	};
	//Effects
	PlaneParticleEffectInitData pid;
	pid.m_ddsFilePath = L"Assets/Image/illumination.dds";
	pid.m_height = 10;
	pid.m_width = 10;
	pid.m_extendDataSize = sizeof(ParticleData);
	pid.m_isBillboard = true;
	//pid.m_isBillboard = false;
	//pid.m_isDepthTest = false;
	PlaneParticleUpdater m_effctUpdater(
		[&]PLANE_PARTICLE_GENERATE_FUNC(pThis, deltaTime)
	{
		static float time = 0;
		//static Vector3 oldPos[2] = { g_vec3Zero };	// エフェクトの前のポジション
		static int footNum = 0;

		//auto pThrust = EnemyManager::GetEnemyManager().GetPlayer()->IsUsingThrusters();
		//if (time >= 0.01f)
		{
			ParticleData pd;

			//エフェクトのQuaternionをゲット
			Quaternion rot = pThis->GetWorldMatrix().GetRotate();
			rot.Inverse(rot);	//エフェクトの逆Quaternion これを使うとワールド座標になる

			//新しいエフェクトの位置から古いエフェクトの位置のベクトル
			Vector3 posv = m_oldPos - pThis->GetWorldMatrix().GetTransrate();

			for (int _i = 0; _i < 10; _i++)
			{
				pd.rnd = GRandom().Rand();

				pd.pos = posv * ((float)_i / 9.f * EFFECT_SCALE_INV);
				rot.Apply(pd.pos);

				//次のフレームのための高さ補間。
				pd.pos.y += (float)_i / 9.f * PARTICLE_Y_UP * deltaTime;

				pThis->AddParticle(pd.pos, g_vec3One * PARTICLE_SCALE, g_quatIdentity, { 3,2.f,0.3,0.5 }, PARTICLE_LIFE_TIME, &pd, true);
			}
			time = 0;
		}

		m_oldPos = pThis->GetWorldMatrix().GetTransrate();
		time += deltaTime;
	},
		[&]PLANE_PARTICLE_UPDATE_FUNC(data, deltaTime, extendData)
	{
		auto& s = *(ParticleData*)extendData;
		data.particleData.pos.y += PARTICLE_Y_UP * deltaTime;

		float n = GPerlinNoise2D().GenerateNoise({ s.rnd * 10, data.particleData.pos.y / 10.f });
		float m = GPerlinNoise2D().GenerateNoise({ data.particleData.pos.y / 10.f, s.rnd * 10 });
		data.particleData.pos.x = s.pos.x + n * 500.f * deltaTime;
		data.particleData.pos.z = s.pos.z + m * 500.f * deltaTime;

		data.particleData.sca = g_vec3One * min((data.lifeTime / PARTICLE_LIFE_TIME) + 0.1f, 1.f) * PARTICLE_SCALE;

		Vector3 col;
		col.Lerp(data.lifeTime / PARTICLE_LIFE_TIME, { 3,0.1f,0.0 }, { 3,1.5f,0.3 });
		data.particleData.mulColor.Set(col);
		data.particleData.mulColor.a = data.lifeTime / PARTICLE_LIFE_TIME;

	});
	pid.m_updater = &m_effctUpdater;

	m_particleEffect = NewGO<PlaneParticleEffectRender>(0);
	m_particleEffect->Init(pid);
	m_particleEffect->SetPos({ 0,0,50 });
	m_particleEffect->SetSca(g_vec3One * EFFECT_SCALE);
}