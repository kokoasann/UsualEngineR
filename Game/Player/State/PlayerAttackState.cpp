#include "stdafx.h"
#include "PlayerAttackState.h"
#include "../Player.h"
#include "../Attack/IPlayerAttack.h"
#include "../Attack/PlayerAttackSlash.h"
#include "../Attack/PlayerAttackA.h"
#include "../Attack/DefaultPreset/PlayerAttackPunch.h"
#include "../Attack/DefaultPreset/PlayerAttackKick.h"
#include "../Attack/DistantPreset/PlayerAttackHipShot.h"
#include "../Attack/DistantPreset/PlayerAttackThrowPod.h"
#include "../Attack/MeleePreset/PlayerAttackRevolvingTackle.h"
#include "../Attack/MeleePreset/PlayerAttackTackle.h"
#include "../Attack/ExplosivePreset/PlayerAttackExplosion.h"
#include "../Attack/ExplosivePreset/PlayerAttackThrowBomb.h"

PlayerAttackState::PlayerAttackState() {

	auto slash = new PlayerAttackSlash;
	m_attackClasses.push_back(slash);

	auto attackA = new PlayerAttackA;
	m_attackClasses.push_back(attackA);

	//Default
	auto punch = new PlayerAttackPunch;
	m_attackClasses.push_back(punch);

	auto kick = new PlayerAttackKick;
	m_attackClasses.push_back(kick);

	//Distant
	auto hipShot = new PlayerAttackHipShot;
	m_attackClasses.push_back(hipShot);

	auto throwPod = new PlayerAttackThrowPod;
	m_attackClasses.push_back(throwPod);

	//Melee
	auto tackle = new PlayerAttackTackle;
	m_attackClasses.push_back(tackle);

	auto revolvingTackle = new PlayerAttackRevolvingTackle;
	m_attackClasses.push_back(revolvingTackle);

	//Explosion
	auto throwBomb = new PlayerAttackThrowBomb;
	m_attackClasses.push_back(throwBomb);

	auto explosion = new PlayerAttackExplosion;
	m_attackClasses.push_back(explosion);

	//Preset Default
	std::vector<IPlayerAttack*> presetDefault;
	presetDefault.resize(TO_INT(EnAttackType::enNumAttackType));
	presetDefault.at(TO_INT(EnAttackType::enNormalAttack)) = punch;
	presetDefault.at(TO_INT(EnAttackType::enSecondAttack)) = kick;
	m_skills.insert(std::make_pair(EnPreset::enPresetDefault, presetDefault));

	//Preset Distant
	std::vector<IPlayerAttack*> presetDistant;
	presetDistant.resize(TO_INT(EnAttackType::enNumAttackType));
	presetDistant.at(TO_INT(EnAttackType::enNormalAttack)) = hipShot;
	presetDistant.at(TO_INT(EnAttackType::enSecondAttack)) = throwPod;
	m_skills.insert(std::make_pair(EnPreset::enPresetDistant, presetDistant));

	//Preset Melee
	std::vector<IPlayerAttack*> presetMelee;
	presetMelee.resize(TO_INT(EnAttackType::enNumAttackType));
	presetMelee.at(TO_INT(EnAttackType::enNormalAttack)) = tackle;
	presetMelee.at(TO_INT(EnAttackType::enSecondAttack)) = revolvingTackle;
	m_skills.insert(std::make_pair(EnPreset::enPresetMelee, presetMelee));

	//Preset Explosion
	std::vector<IPlayerAttack*> presetExplosion;
	presetExplosion.resize(TO_INT(EnAttackType::enNumAttackType));
	presetExplosion.at(TO_INT(EnAttackType::enNormalAttack)) = throwBomb;
	presetExplosion.at(TO_INT(EnAttackType::enSecondAttack)) = explosion;
	m_skills.insert(std::make_pair(EnPreset::enPresetExplosion, presetExplosion));

}

PlayerAttackState::~PlayerAttackState() {
	for (int i = 0; i < m_attackClasses.size(); i++) {
		delete m_attackClasses[i];
	}
}

void PlayerAttackState::Enter(Player* p) {

#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player Attack Enter\n");
#endif

	auto& vel = p->GetLocalVelocity();
	p->SetVelocity(Vector3::Zero);
	m_combo = 1;

	auto& preset = p->GetCurrentAttackPreset();

	//Default Preset
	if (preset == Player::EnAttackPreset::enDefault) {

		auto& defaultSkills = m_skills.at(EnPreset::enPresetDefault);

		if (g_pad[0]->IsTrigger(enButtonB)) {
			m_currentAttack = defaultSkills.at(TO_INT(EnAttackType::enNormalAttack));
		}

		if (g_pad[0]->IsTrigger(enButtonY)) {
			m_currentAttack = defaultSkills.at(TO_INT(EnAttackType::enSecondAttack));
		}
	}

	//Preset A
	if (preset == Player::EnAttackPreset::enRemoteAttackPreset) {

		auto& ASkills = m_skills.at(EnPreset::enPresetDistant);

		if (g_pad[0]->IsTrigger(enButtonB)) {
			m_currentAttack = ASkills.at(TO_INT(EnAttackType::enNormalAttack));
		}

		if (g_pad[0]->IsTrigger(enButtonY)) {
			m_currentAttack = ASkills.at(TO_INT(EnAttackType::enSecondAttack));
		}
	}


	//Preset B
	if (preset == Player::EnAttackPreset::enMeleePreset) {

		auto& BSkills = m_skills.at(EnPreset::enPresetMelee);

		if (g_pad[0]->IsTrigger(enButtonB)) {
			m_currentAttack = BSkills.at(TO_INT(EnAttackType::enNormalAttack));
		}

		if (g_pad[0]->IsTrigger(enButtonY)) {
			m_currentAttack = BSkills.at(TO_INT(EnAttackType::enSecondAttack));
		}
	}

	//Preset C
	if (preset == Player::EnAttackPreset::enExplosivePreset) {

		auto& CSkills = m_skills.at(EnPreset::enPresetExplosion);

		if (g_pad[0]->IsTrigger(enButtonB)) {
			m_currentAttack = CSkills.at(TO_INT(EnAttackType::enNormalAttack));
		}

		if (g_pad[0]->IsTrigger(enButtonY)) {
			m_currentAttack = CSkills.at(TO_INT(EnAttackType::enSecondAttack));
		}
	}

	m_currentAttack->Init(p,m_combo);
	m_currentAttack->GetInterval();
}

void PlayerAttackState::Exit(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Attack Exit\n");
#endif
}

IPlayerState* PlayerAttackState::Update(Player* p) {

	m_currentAttack->Execute(p);
	
	if (m_currentAttack->IsDone()) {
		if (m_elpased <= m_currentAttack->GetInterval()) {
			m_elpased += gameTime()->GetDeltaTime();
			return this;
		}
		auto ca = m_currentAttack->IsContinueAttack();
		if (ca and m_combo < m_MAX_COMBO) {
			m_combo++;
			m_currentAttack->Init(p,m_combo);
		}
		else {
			return p->GetPreviousState();
		}
	}

	return this;
}