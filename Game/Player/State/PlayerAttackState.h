#pragma once
#include "IPlayerSstate.h"
class IPlayerAttack;
//for normal attack on ground

class PlayerAttackState : public IPlayerState
{
public:
	PlayerAttackState();
	~PlayerAttackState();
	void Enter(Player*) override;
	IPlayerState* Update(Player*) override;
	void Exit(Player*) override;
private:
	IPlayerAttack* m_currentAttack = nullptr;

	enum class EnPreset {
		enPresetDefault,
		enPresetA,
		enPresetB,
		enPresetC,
		enNumPreset
	};

	enum class EnAttackType {
		enNormalAttack,
		enSecondAttack,
		//enSubAttack,
		enNumAttackType
	};

	std::map<EnPreset, std::vector<IPlayerAttack*>> m_skills;

	int m_combo = 1;
	const int m_MAX_COMBO = 3;
	float m_elpased = 0.f;
};

