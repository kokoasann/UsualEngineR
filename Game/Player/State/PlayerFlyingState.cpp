#include "stdafx.h"
#include "PlayerFlyingState.h"
#include "../Player.h"

PlayerFlyingState::PlayerFlyingState()
{

}

PlayerFlyingState::~PlayerFlyingState()
{

}

void PlayerFlyingState::Enter(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player Enter Flying\n");
#endif

	m_canRise = true;
	//ブーストの上昇初期コストがないので飛べないよ.
	if (p->GetCurrentBoost() <= m_AUTO_RISE_BOOST_COST) {
		m_canRise = false;
		return;
	}

	m_bPos = p->GetPosition();
	m_velocity = p->GetLocalVelocity();
	m_velocity.y = 0.f;

	if (p->IsOnGround()) {
		m_velocityGoal.y = m_AUTO_RISE_PARAM * m_VELOCITY_MAX;
		m_isFirstRising = true;
	}

	p->PlayAnimation(Player::EnAnimation::enIdle);


	//Effects
	PlaneParticleEffectInitData pid;
	pid.m_ddsFilePath = L"Assets/Image/illumination.dds";
	pid.m_height = 10;
	pid.m_width = 10;
	pid.m_extendDataSize = sizeof(float);
	pid.m_isBillboard = true;
	//pid.m_isBillboard = false;
	//pid.m_isDepthTest = false;
	PlaneParticleUpdater m_effctUpdater(
		[&]PLANE_PARTICLE_GENERATE_FUNC(pThis, deltaTime)
	{
		static float time = 0;
		if (time >= 0.01f)
		{
			//Matrix m = g_matIdentity;
			//pThis->AddParticle(m, { 1,1,1,1 }, 10);
			for (int _i = 0; _i < 3; _i++)
			{
				float i = GRandom().Rand();
				pThis->AddParticle(g_vec3Zero, g_vec3One, g_quatIdentity, { 3,2.f,0.3,1 }, 10, &i);
			}
			time = 0;
		}
		time += deltaTime;
	},
		[&]PLANE_PARTICLE_UPDATE_FUNC(data, deltaTime, extendData)
	{
		auto s = *(float*)extendData;
		data.particleData.pos.y += 30.f * deltaTime;

		float n = GPerlinNoise2D().GenerateNoise({ s * 10, data.particleData.pos.y / 10.f });
		float m = GPerlinNoise2D().GenerateNoise({ data.particleData.pos.y / 10.f, s * 10 });
		data.particleData.pos.x = n * 500.f * deltaTime;
		data.particleData.pos.z = m * 500.f * deltaTime;
		data.particleData.sca = g_vec3One * min((data.lifeTime / 10.f) + 0.1f, 1.f);

		Vector3 col;
		col.Lerp(data.lifeTime / 10.f, { 3,0.1f,0.0 }, { 3,1.5f,0.3 });
		data.particleData.mulColor.Set(col);
		data.particleData.mulColor.a = data.lifeTime / 10.f;

	});
	pid.m_updater = &m_effctUpdater;

	m_effect = NewGO<PlaneParticleEffectRender>(0);
	m_effect->Init(pid);
	m_effect->SetPos({ 0,0,50 });
	m_effect->SetSca(g_vec3One * 0.02);

	m_effect1 = NewGO<PlaneParticleEffectRender>(0);
	m_effect1->Init(pid);
	m_effect1->SetPos({ 0,0,50 });
	m_effect1->SetSca(g_vec3One * 0.02);

}

IPlayerState*  PlayerFlyingState::Update(Player* p) {

	//State
	if (g_pad[0]->IsTrigger(EnButton::enButtonB) or g_pad[0]->IsTrigger(EnButton::enButtonY)) {
		auto nextState = p->GetState(Player::EnState::enAttack);
		return nextState;
	}

	if (g_pad[0]->IsTrigger(EnButton::enButtonA) or  //飛行モード解除.
		p->GetCurrentBoost() <= 0.f or //ブースト不足.
		!m_canRise //初期上昇用のブーストが不足.
		) {
		auto nextState = p->GetState(Player::EnState::enGround);
		return nextState;
	}
	
	//Move
	auto lxf = g_pad[0]->GetLStickXF();
	auto lyf = g_pad[0]->GetLStickYF();
	auto r2f = g_pad[0]->GetR2Button();

	if (p->GetPosition().y > m_bPos.y + m_rise and m_isFirstRising) {
		m_velocityGoal.y = 0.f;
		m_isFirstRising = false;
	}

	if (!m_isFirstRising) {
		m_velocityGoal.y = r2f * m_VELOCITY_MAX;
	}

	m_velocityGoal.x = lxf * m_VELOCITY_MAX;
	m_velocityGoal.z = lyf * m_VELOCITY_MAX;


	//BOOST
	if (g_pad[0]->IsPress(enButtonX)) {
		m_velocityGoal *= m_VELOCITY_BOOST;
		p->UseBoost(m_BOOST_EFFICIENCY * m_ACCELERATE_PARAM * gameTime()->GetDeltaTime());

	}
	else {
		p->UseBoost(m_BOOST_EFFICIENCY * gameTime()->GetDeltaTime());
	}

	//上昇してるなら追加でブーストを消費
	if (m_velocityGoal.y > 0.f) {
		p->UseBoost(m_BOOST_EFFICIENCY * m_RISE_BOOST_PARAM * gameTime()->GetDeltaTime());
	}

	auto delta = gameTime()->GetDeltaTime();

	auto cam = g_lockCamera3D.Get();
	auto forward = cam->GetForward();
	forward.y = 0.f;
	forward.Normalize();
	Vector3 up = { 0,1,0 };
	Vector3 right = forward;
	right.Cross(up);
	right.Normalize();

	m_velocity.x = Approach(m_velocityGoal.x, m_velocity.x, delta * m_QUICKNESS);
	m_velocity.z = Approach(m_velocityGoal.z, m_velocity.z, delta * m_QUICKNESS);
	m_velocity.y = Approach(m_velocityGoal.y, m_velocity.y, delta * m_QUICKNESS);

	auto vel = forward * m_velocity.z + right * -m_velocity.x + up * m_velocity.y;

	vel *= p->GetSpeed();
	p->SetVelocity(vel);
	p->SetLocalVelocity(m_velocity);

	//Rotation
	if (vel.x != 0.f or vel.z != 0.f) {
		Quaternion rot = Quaternion::Identity;
		auto theta = atan2(vel.x, vel.z);
		theta = theta * (180.f / Math::PI);
		rot.SetRotationDegY(theta);
		p->SetRotation(rot);
	}

	//Effect
	const auto& boneSoleLMat = p->GetBone(Player::EnPlayerBone::enSOLE_L)->GetWorldMatrix();
	m_effect->SetRot(boneSoleLMat.GetRotate());
	m_effect->SetPos(boneSoleLMat.GetTransrate());


	const auto& boneSoleRMat = p->GetBone(Player::EnPlayerBone::enSOLE_R)->GetWorldMatrix();
	m_effect1->SetRot(boneSoleRMat.GetRotate());
	m_effect1->SetPos(boneSoleRMat.GetTransrate());

	return this;
}

void PlayerFlyingState::Exit(Player* p) {
	DeleteGO(m_effect);
	DeleteGO(m_effect1);
	m_velocity = Vector3::Zero;
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player Exit Flying\n");
#endif
}