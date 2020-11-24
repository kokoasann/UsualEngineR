#include "stdafx.h"
#include <algorithm>
#include "Player.h"
#include "State/PlayerFlyingState.h"
#include "State/PlayerGroundState.h"
#include "State/PlayerFallState.h"
#include "State/PlayerDiveState.h"
#include "State/PlayerAttackState.h"
#include "State/PlayerGuardState.h"
#include "State/PlayerDeadState.h"
#include "State/PlayerStunState.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/IEnemy.h"
#include "Pod/Pod.h"
#include "../GameManager.h"
#include "../GameSceneMenu.h"
#include "Attachment/JetPack.h"
#include "../Effect/JetEffect.h"

Player::Player()
{
}

Player::~Player()
{
}



void Player::Release()
{
	std::for_each(m_stateList.begin(), m_stateList.end(), [](IPlayerState* state) { delete state; });
	DeleteGO(m_model);
	DeleteGO(m_pod);
	DeleteGO(m_jetPack);

	for (int i = 0; i < m_jetEffects.size(); i++) {
		DeleteGO(m_jetEffects[i]);
	}
	//DeleteGO(m_thrusterEffects[RIGHT]);
	//DeleteGO(m_thrusterEffects[LEFT]);
}

void Player::OnDestroy()
{
	Release();
}


void Player::Awake()
{

	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/m/m_ExBone.tkm";
	mid.m_tksFilePath = "Assets/modelData/m/m_ExBone.tks";
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(m_scale);
	//m_model->SetMulColor({ 0.5, 0.5, 0.4, 1.f });

	//
	/*
	ModelInitData mid;
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	mid.m_tksFilePath = "Assets/modelData/unityChan.tks";
	mid.m_upAxis = enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(Vector3::One * m_scale);
	*/

	//Init Animation
	const int numAnim = static_cast<int>(EnAnimation::enNumAnim);
	m_animlist.resize(numAnim);

	//Idle
	m_animlist[static_cast<int>(EnAnimation::enIdle)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enIdle)]->Load("Assets/modelData/m/anim/m_idle.tka");
	m_animlist[static_cast<int>(EnAnimation::enIdle)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enIdle)]->SetLoopFlag(true);



	//Run
	m_animlist[static_cast<int>(EnAnimation::enRun)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enRun)]->Load("Assets/modelData/m/anim/m_dash.tka");
	m_animlist[static_cast<int>(EnAnimation::enRun)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enRun)]->SetLoopFlag(true);

	//Walk
	m_animlist[static_cast<int>(EnAnimation::enWalk)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enWalk)]->Load("Assets/modelData/m/anim/m_walk.tka");
	m_animlist[static_cast<int>(EnAnimation::enWalk)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enWalk)]->SetLoopFlag(true);

	//Hovering
	m_animlist[static_cast<int>(EnAnimation::enHovering)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enHovering)]->Load("Assets/modelData/m/anim/m_fly.tka");
	m_animlist[static_cast<int>(EnAnimation::enHovering)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enHovering)]->SetLoopFlag(true);

	//FlyMove
	m_animlist[static_cast<int>(EnAnimation::enFlyMove)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enFlyMove)]->Load("Assets/modelData/m/anim/m_fly_move.tka");
	m_animlist[static_cast<int>(EnAnimation::enFlyMove)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enFlyMove)]->SetLoopFlag(true);

	//FlyMoveFaster
	m_animlist[static_cast<int>(EnAnimation::enFlyMoveFaster)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enFlyMoveFaster)]->Load("Assets/modelData/m/anim/m_fly_move_dash.tka");
	m_animlist[static_cast<int>(EnAnimation::enFlyMoveFaster)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enFlyMoveFaster)]->SetLoopFlag(true);

	//TODO : load got damage anim
	//KnockBack
	m_animlist[static_cast<int>(EnAnimation::enKnockBack)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enKnockBack)]->Load("Assets/modelData/m/anim/m_hit.tka");
	m_animlist[static_cast<int>(EnAnimation::enKnockBack)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enKnockBack)]->SetLoopFlag(false);

	//Guard
	m_animlist[static_cast<int>(EnAnimation::enGuard)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enGuard)]->Load("Assets/modelData/m/anim/m_guard.tka");
	m_animlist[static_cast<int>(EnAnimation::enGuard)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enGuard)]->SetLoopFlag(false);

	//Attack
	m_animlist[static_cast<int>(EnAnimation::enAttack)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enAttack)]->Load("Assets/modelData/m/anim/m_punch_1.tka");
	m_animlist[static_cast<int>(EnAnimation::enAttack)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enAttack)]->SetLoopFlag(false);

	//Punch1
	m_animlist[static_cast<int>(EnAnimation::enPunch1)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enPunch1)]->Load("Assets/modelData/m/anim/m_punch_1.tka");
	m_animlist[static_cast<int>(EnAnimation::enPunch1)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enPunch1)]->SetLoopFlag(false);

	//Punch2
	m_animlist[static_cast<int>(EnAnimation::enPunch2)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enPunch2)]->Load("Assets/modelData/m/anim/m_punch_2.tka");
	m_animlist[static_cast<int>(EnAnimation::enPunch2)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enPunch2)]->SetLoopFlag(false);

	//Punch3
	m_animlist[static_cast<int>(EnAnimation::enPunch3)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enPunch3)]->Load("Assets/modelData/m/anim/m_punch_3.tka");
	m_animlist[static_cast<int>(EnAnimation::enPunch3)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enPunch3)]->SetLoopFlag(false);

	//Kick
	m_animlist[static_cast<int>(EnAnimation::enKick)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enKick)]->Load("Assets/modelData/m/anim/m_rolling_kick.tka");
	m_animlist[static_cast<int>(EnAnimation::enKick)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enKick)]->SetLoopFlag(false);

	//TODO : load dead anim
	//Dead
	m_animlist[static_cast<int>(EnAnimation::enDead)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enDead)]->Load("Assets/modelData/m/anim/m_idle.tka");
	m_animlist[static_cast<int>(EnAnimation::enDead)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enDead)]->SetLoopFlag(false);

	m_model->InitAnimation(m_animlist, m_animlist.size());
	m_model->Play(0);
}

bool Player::Start()
{

	//State
	m_stateList.resize(static_cast<int>(EnState::enNumState));
	m_stateList[static_cast<int>(EnState::enGround)] = new PlayerGroundState();
	m_stateList[static_cast<int>(EnState::enFlying)] = new PlayerFlyingState();
	m_stateList[static_cast<int>(EnState::enFall)] = new PlayerFallState();
	m_stateList[static_cast<int>(EnState::enDiving)] = new PlayerDiveState();
	m_stateList[static_cast<int>(EnState::enAttack)] = new PlayerAttackState();
	m_stateList[static_cast<int>(EnState::enDead)] = new PlayerDeadState();
	m_stateList[static_cast<int>(EnState::enStun)] = new PlayerStunState();
	m_stateList[static_cast<int>(EnState::enGuard)] = new PlayerGuardState();

	m_currentState = m_nextState = m_stateList[static_cast<int>(EnState::enFlying)];
	m_nextState->Enter(this);

	//Physics
	m_charaCon.Init(m_charaConRadius, m_charaConHeight, m_position, /*isUseRigidBody */ true);
	m_charaCon.AddCollisionAttribute(GameCollisionAttribute::Player);

	//Pod
	m_pod = NewGO<Pod>(0);
	m_pod->SetPlayer(this);

	//Attachment
	m_playerBones.resize(TO_INT(EnPlayerBone::enNumBoneType));

	m_jetPack = NewGO<JetPack>(0);
	m_jetPack->SetUsingState(false);

	auto bone = this->GetModel().GetSkelton()->GetBone(this->GetModel().GetSkelton()->FindBoneID(L"Bone.005"));
	m_playerBones.at(TO_INT(EnPlayerBone::enBack)) = bone;

	auto soleR = this->GetModel().GetSkelton()->GetBone(this->GetModel().GetSkelton()->FindBoneID(L"Sole_R"));
	m_playerBones.at(TO_INT(EnPlayerBone::enSOLE_R)) = soleR;

	auto soleL = this->GetModel().GetSkelton()->GetBone(this->GetModel().GetSkelton()->FindBoneID(L"Sole_L"));
	m_playerBones.at(TO_INT(EnPlayerBone::enSOLE_L)) = soleL;
	
	//Effect
	auto jetEffect = NewGO<JetEffect>(0);
	auto jetEffect1 = NewGO<JetEffect>(0);
	JetEffect::JetEffectInitParam jeip;
	jeip.effectScale = 0.01f;
	jeip.effectScale_inv = 100.f;
	jeip.particleScale = 5.f;
	jeip.particleLifeTime = 1.f;
	jeip.particleYUp = 300.f;
	jetEffect->Init(jeip);
	jetEffect1->Init(jeip);
	m_jetEffects.push_back(jetEffect);
	m_jetEffects.push_back(jetEffect1);

	return true;
}


void Player::PreUpdate()
{
	if (GameManager::GetInstance().m_menu->IsGamePaused()) return;

	m_charaCon.Execute(gameTime()->GetDeltaTime(), m_velocity);

	//DebugPrintVector3(EDebugConsoloUser::WATA, m_velocity);

	m_position = m_charaCon.GetPosition();
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
}

void Player::Update()
{
	if (GameManager::GetInstance().m_menu->IsGamePaused()) return;

	//Attachments
	//Jetpack
	if (m_currentAttackPreset == EnAttackPreset::enMeleePreset) {
		m_jetPack->SetUsingState(true);
		const auto& mat = m_playerBones.at(TO_INT(EnPlayerBone::enBack))->GetWorldMatrix();
		m_jetPack->SetPosition(mat.GetTransrate());
		m_jetPack->SetRotation(mat.GetRotate());
	}
	else {
		m_jetPack->SetUsingState(false);
	}


	SearchTarget();
	UpdateAttackType();

	//Calc forward
	auto& world = m_model->GetModel().GetWorldMatrix();
	m_forward.x = world.mat._31;
	m_forward.y = world.mat._32;
	m_forward.z = world.mat._33;
	m_forward.Normalize();

	m_nextState = m_currentState->Update(this);

	if (m_hp <= 0) {
		m_nextState = m_stateList[static_cast<int>(EnState::enDead)];
	}

	if (m_nextState != m_currentState) {
		m_currentState->Exit(this);
		m_previousState = m_currentState;
		m_currentState = m_nextState;
		m_currentState->Enter(this);
	}
}

void Player::PostUpdate()
{
	//Effect
	if (!m_isUsingThrusters) {

		m_jetEffects[LEFT]->SetGenerateFlag(false);
		m_jetEffects[RIGHT]->SetGenerateFlag(false);
		//m_thrusterEffects[LEFT]->SetActive(false);
		//m_thrusterEffects[RIGHT]->SetActive(false);
		m_jetPack->StopThrusters();
		//return;
	}
	else {
		m_jetEffects[LEFT]->SetGenerateFlag(true);
		m_jetEffects[RIGHT]->SetGenerateFlag(true);
		m_jetPack->FireThrusters();
	}

	//m_thrusterEffects[LEFT]->SetActive(true);
	//m_thrusterEffects[RIGHT]->SetActive(true);

	m_jetEffects[RIGHT]->SetPosition(GetBone(Player::EnPlayerBone::enSOLE_R)->GetWorldMatrix().GetTransrate());
	m_jetEffects[RIGHT]->SetRotation(GetBone(Player::EnPlayerBone::enSOLE_R)->GetWorldMatrix().GetRotate());
	m_jetEffects[LEFT]->SetPosition(GetBone(Player::EnPlayerBone::enSOLE_L)->GetWorldMatrix().GetTransrate());
	m_jetEffects[LEFT]->SetRotation(GetBone(Player::EnPlayerBone::enSOLE_L)->GetWorldMatrix().GetRotate());

	/*
	const auto& boneSoleLMat = GetBone(Player::EnPlayerBone::enSOLE_L)->GetWorldMatrix();
	m_thrusterEffects[LEFT]->SetRot(boneSoleLMat.GetRotate());
	m_thrusterEffects[LEFT]->SetPos(boneSoleLMat.GetTransrate());

	const auto& boneSoleRMat = GetBone(Player::EnPlayerBone::enSOLE_R)->GetWorldMatrix();
	m_thrusterEffects[RIGHT]->SetRot(boneSoleRMat.GetRotate());
	m_thrusterEffects[RIGHT]->SetPos(boneSoleRMat.GetTransrate());
	*/
}



void Player::Render()
{

}

void Player::PostRender()
{

}

void Player::SearchTarget() {
	auto enemy = EnemyManager::GetEnemyManager().GetNearestEnemy();
	if (enemy != nullptr /*and enemy->IsBoss()*/ and (enemy->GetState(IEnemy::EnState::enIdleState) != enemy->GetCurrentState() or
		enemy->GetState(IEnemy::EnState::enDeadState) != enemy->GetCurrentState())) {
		m_target = enemy;
	}
	else {
		m_target = nullptr;
	}
}

void Player::UpdateAttackType() {

	if (g_pad[0]->IsTrigger(enButtonLeft)) {
		if (m_canUsePreset[TO_INT(EnAttackPreset::enRemoteAttackPreset)]) {
			m_currentAttackPreset = EnAttackPreset::enRemoteAttackPreset;
		}
	}

	if (g_pad[0]->IsTrigger(enButtonUp)) {
		if (m_canUsePreset[TO_INT(EnAttackPreset::enMeleePreset)])
			m_currentAttackPreset = EnAttackPreset::enMeleePreset;
	}

	if (g_pad[0]->IsTrigger(enButtonRight)) {
		if (m_canUsePreset[TO_INT(EnAttackPreset::enExposivePreset)])
			m_currentAttackPreset = EnAttackPreset::enExposivePreset;
	}

	if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_currentAttackPreset = EnAttackPreset::enDefault;
	}
}

//void Player::InitThrusterEffect() {
	//const static float EFFECT_SCALE = 0.01f;
	//const static float EFFECT_SCALE_INV = 100.f;

	//const static float PERTICLE_SCALE = 500.f;
	//const static float PERTICLE_LIFE_TIME = 1.f;
	//const static float PERTICLE_Y_UP = 300.f;

	////パーティクルの拡張構造体。
	//struct ParticleData
	//{
	//	float rnd;		//パーリンノイズで使うためのseed値的なもの
	//	Vector3 pos;	//パーティクルの初期位置(ローカル)
	//};
	////Effects
	//PlaneParticleEffectInitData pid;
	//pid.m_ddsFilePath = L"Assets/Image/illumination.dds";
	//pid.m_height = 10;
	//pid.m_width = 10;
	//pid.m_extendDataSize = sizeof(ParticleData);
	//pid.m_isBillboard = true;
	////pid.m_isBillboard = false;
	////pid.m_isDepthTest = false;
	//PlaneParticleUpdater m_effctUpdater(
	//	[&]PLANE_PARTICLE_GENERATE_FUNC(pThis, deltaTime)
	//{
	//	static float time = 0;
	//	static Vector3 oldPos[2] = { g_vec3Zero };	// エフェクトの前のポジション
	//	static int footNum = 0;

	//	auto pThrust = EnemyManager::GetEnemyManager().GetPlayer()->IsUsingThrusters();
	//	if (time >= 0.01f and pThrust)
	//	{
	//		ParticleData pd;

	//		//エフェクトのQuaternionをゲット
	//		Quaternion rot = pThis->GetWorldMatrix().GetRotate();
	//		rot.Inverse(rot);	//エフェクトの逆Quaternion これを使うとワールド座標になる

	//		//新しいエフェクトの位置から古いエフェクトの位置のベクトル
	//		Vector3 posv = oldPos[footNum] - pThis->GetWorldMatrix().GetTransrate();

	//		for (int _i = 0; _i < 50; _i++)
	//		{
	//			pd.rnd = GRandom().Rand();

	//			pd.pos = posv* ((float)_i / 49.f * EFFECT_SCALE_INV);
	//			rot.Apply(pd.pos);

	//			//次のフレームのための高さ補間。
	//			pd.pos.y += (float)_i / 49.f * PERTICLE_Y_UP * deltaTime;
	//			
	//			pThis->AddParticle(pd.pos, g_vec3One * PERTICLE_SCALE, g_quatIdentity, { 3,2.f,0.3,0.5 }, PERTICLE_LIFE_TIME, &pd, true);
	//		}
	//		time = 0;
	//	}

	//	oldPos[footNum] = pThis->GetWorldMatrix().GetTransrate();
	//	footNum ^= 1;
	//	time += deltaTime;
	//},
	//	[&]PLANE_PARTICLE_UPDATE_FUNC(data, deltaTime, extendData)
	//{
	//	auto& s = *(ParticleData*)extendData;
	//	data.particleData.pos.y += PERTICLE_Y_UP * deltaTime;

	//	float n = GPerlinNoise2D().GenerateNoise({ s.rnd * 10, data.particleData.pos.y / 10.f });
	//	float m = GPerlinNoise2D().GenerateNoise({ data.particleData.pos.y / 10.f, s.rnd * 10 });
	//	data.particleData.pos.x = s.pos.x + n * 500.f * deltaTime;
	//	data.particleData.pos.z = s.pos.z + m * 500.f * deltaTime;

	//	data.particleData.sca = g_vec3One * min((data.lifeTime / PERTICLE_LIFE_TIME) + 0.1f, 1.f) * 5.f;

	//	Vector3 col;
	//	col.Lerp(data.lifeTime / PERTICLE_LIFE_TIME, { 3,0.1f,0.0 }, { 3,1.5f,0.3 });
	//	data.particleData.mulColor.Set(col);
	//	data.particleData.mulColor.a = data.lifeTime / PERTICLE_LIFE_TIME;

	//});
	//pid.m_updater = &m_effctUpdater;

	//m_thrusterEffects[RIGHT] = NewGO<PlaneParticleEffectRender>(0);
	//m_thrusterEffects[RIGHT]->Init(pid);
	//m_thrusterEffects[RIGHT]->SetPos({ 0,0,50 });
	//m_thrusterEffects[RIGHT]->SetSca(g_vec3One * EFFECT_SCALE);

	//m_thrusterEffects[LEFT] = NewGO<PlaneParticleEffectRender>(0);
	//m_thrusterEffects[LEFT]->Init(pid);
	//m_thrusterEffects[LEFT]->SetPos({ 0,0,50 });
	//m_thrusterEffects[LEFT]->SetSca(g_vec3One * EFFECT_SCALE);
//}