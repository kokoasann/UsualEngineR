#include "stdafx.h"
#include "RedPotion.h"
#include "../Player/Player.h"

RedPotion::RedPotion() {
}

RedPotion::~RedPotion() {

}

void RedPotion::Apply(Player* player) {
	player->Heal(m_healAmount);
}

