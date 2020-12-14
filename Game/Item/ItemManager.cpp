#include "stdafx.h"
#include "ItemManager.h"
#include "IItem.h"
#include "../GameManager.h"
#include "../Player/Player.h"
#include "RedPotion.h"
#include "EnergyPotion.h"

ItemManager::ItemManager()
{

}

ItemManager::~ItemManager()
{

}



void ItemManager::Release()
{
	for (int i = 0; i < m_items.size(); i++) {
		delete m_items[i];
	}
}

void ItemManager::OnDestroy()
{
	Release();
}


void ItemManager::Awake()
{

}

bool ItemManager::Start()
{

	auto item = new RedPotion();
	m_items.push_back(item);

	auto energy = new EnergyPotion();
	energy->SetPosition({ 150,0,0. });
	m_items.push_back(energy);

	return true;
}


void ItemManager::PreUpdate()
{

}

void ItemManager::Update()
{
	for (int i = 0; i < m_items.size(); i++) {
		auto player = GameManager::GetInstance().m_player;
		if (player == nullptr) return;

		m_items.at(i)->Update();

		auto dist = (player->GetPosition() - m_items[i]->GetPosition()).Length();
		if (dist < m_items[i]->GetRange()) {
			m_items[i]->Apply(player);

			//delete the item then erase it from my vector of items
			delete m_items[i];
			m_items.erase(std::remove(m_items.begin(), m_items.end(), m_items[i]), m_items.end());
		}
	}
}

void ItemManager::PostUpdate()
{

}


void ItemManager::Render()
{

}

void ItemManager::PostRender()
{

}
