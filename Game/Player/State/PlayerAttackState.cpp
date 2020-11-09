#include "stdafx.h"
#include "PlayerAttackState.h"
#include "../Player.h"
#include "../Attack/IPlayerAttack.h"
#include "../Attack/PlayerAttackSlash.h"
#include "../Attack/PlayerAttackA.h"

PlayerAttackState::PlayerAttackState() {

	auto slash = new PlayerAttackSlash;
	auto attackA = new PlayerAttackA;

	m_attackClasses.push_back(slash);
	m_attackClasses.push_back(attackA);

	//Preset0
	std::vector<IPlayerAttack*> presetDefault;
	presetDefault.resize(TO_INT(EnAttackType::enNumAttackType));
	presetDefault.at(TO_INT(EnAttackType::enNormalAttack)) = slash;
	presetDefault.at(TO_INT(EnAttackType::enSecondAttack)) = attackA;
	m_skills.insert(std::make_pair(EnPreset::enPresetDefault, presetDefault));

	//Preset1
	std::vector<IPlayerAttack*> presetA;
	presetA.resize(TO_INT(EnAttackType::enNumAttackType));
	presetA.at(TO_INT(EnAttackType::enNormalAttack)) = attackA;
	presetA.at(TO_INT(EnAttackType::enSecondAttack)) = slash;
	m_skills.insert(std::make_pair(EnPreset::enPresetA, presetA));

	//Preset2
	std::vector<IPlayerAttack*> presetB;
	presetB.resize(TO_INT(EnAttackType::enNumAttackType));
	presetB.at(TO_INT(EnAttackType::enNormalAttack)) = attackA;
	presetB.at(TO_INT(EnAttackType::enSecondAttack)) = slash;
	m_skills.insert(std::make_pair(EnPreset::enPresetB, presetB));

	//Preset3
	std::vector<IPlayerAttack*> presetC;
	presetC.resize(TO_INT(EnAttackType::enNumAttackType));
	presetC.at(TO_INT(EnAttackType::enNormalAttack)) = attackA;
	presetC.at(TO_INT(EnAttackType::enSecondAttack)) = slash;
	m_skills.insert(std::make_pair(EnPreset::enPresetC, presetC));

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
	if (preset == Player::EnAttackPreset::enA) {

		auto& ASkills = m_skills.at(EnPreset::enPresetA);

		if (g_pad[0]->IsTrigger(enButtonB)) {
			m_currentAttack = ASkills.at(TO_INT(EnAttackType::enNormalAttack));
		}

		if (g_pad[0]->IsTrigger(enButtonY)) {
			m_currentAttack = ASkills.at(TO_INT(EnAttackType::enSecondAttack));
		}
	}


	//Preset B
	if (preset == Player::EnAttackPreset::enB) {

		auto& BSkills = m_skills.at(EnPreset::enPresetB);

		if (g_pad[0]->IsTrigger(enButtonB)) {
			m_currentAttack = BSkills.at(TO_INT(EnAttackType::enNormalAttack));
		}

		if (g_pad[0]->IsTrigger(enButtonY)) {
			m_currentAttack = BSkills.at(TO_INT(EnAttackType::enSecondAttack));
		}
	}

	//Preset C
	if (preset == Player::EnAttackPreset::enC) {

		auto& CSkills = m_skills.at(EnPreset::enPresetC);

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