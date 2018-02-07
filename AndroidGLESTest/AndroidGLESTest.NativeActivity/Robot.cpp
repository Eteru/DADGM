#include "Robot.h"
#include "SceneObjectSpawner.h"
#include "UniqueID.h"
#include "BoundingSphere.h"

#include "PhysicsBody.h"
#include "MapManager.h"
#include "MovementController.h"
#include "ItemDescriptions.h"

Robot::Robot()
	: m_armor(nullptr), m_weapon(nullptr)
{

}

void Robot::Init()
{
	m_target = nullptr;
	m_inventorySize = 3;

	m_equippedItems = std::vector<Item *>(m_inventorySize, nullptr);


}

void Robot::FixedUpdate()
{
	AcquireTarget();
	ManageBuffs();
	

	if (nullptr != m_physicsBody)
	{
		m_physicsBody->m_acceleration = m_armor->m_stats.at(StatType::LINEAR_ACCEL).GetValue();
		m_physicsBody->m_topSpeed = m_armor->m_stats.at(StatType::LINEAR_TOP).GetValue();
		m_physicsBody->m_turningAcceleration = m_armor->m_stats.at(StatType::ANGULAR_ACCEL).GetValue();
		m_physicsBody->m_topTurningSpeed = m_armor->m_stats.at(StatType::ANGULAR_TOP).GetValue();
	}

	m_weapon->m_debugDraw = true;
}

void Robot::Update()
{
// 	//PrintUtils::PrintD("pos: " + m_transform.GetWorldPos().ToString());
// 
// 	if (nullptr != m_visualBody && nullptr != m_physicsBody)
// 	{
// 		m_visualBody->m_transform.SetPos(m_physicsBody->m_transform.GetLocalLerpPos());
// 	}
}

void Robot::Draw()
{
	
}

void Robot::Destroy()
{
// 	if (nullptr != m_armor)
// 	{
// 		delete m_armor;
// 		m_armor = nullptr;
// 	}
// 
// 	if (nullptr != m_weapon)
// 	{
// 		delete m_weapon;
// 		m_weapon = nullptr;
// 	}
}

std::string Robot::ToString()
{
	return std::string("TODO Robot string");
}

std::string Robot::GetClassName()
{
	return std::string("Robot");
}

void Robot::TakeDamage(const float value)
{
	m_stats.at(StatType::HEALTH).AddFlat(-value * (1.f - Math::Clamp(m_armor->m_stats.at(StatType::ARMOR).GetValue(), 0, 1)));
}

bool Robot::IsDead()
{
	return m_stats.at(StatType::HEALTH).GetValue() <= 0.f;
}

void Robot::ActivateItem(const std::string itemName)
{
	ActiveItem *item = FindActive(itemName);

	item->Activate();

	if (nullptr == item)
	{
		PrintUtils::PrintD("Could not find item " + itemName);
		return;
	}

	ApplyModifiers(item->m_modifiers);

	if (item->m_buff.m_duration > 0)
	{
		ApplyBuff(item->m_buff);
	}
}

void Robot::EquipItem(const std::string itemName, const size_t slot, std::vector<StatModifier> extraModifiers /*= std::vector<StatModifier>()*/)
{
	if (slot < 0 || slot > m_inventorySize)
	{
		PrintUtils::PrintD("Invalid slot number " + PrintUtils::ToString(slot));
		return;
	}


	std::pair<bool, Item *> itemPair = ItemDescriptions::GetInstance().SpawnItem(itemName);

	if (nullptr == itemPair.second)
	{
		PrintUtils::PrintD("Unknown item " + itemName);
		return;
	}

	UnequipItem(slot);

	itemPair.second->m_robot = this;
	if (!extraModifiers.empty())
	{
		itemPair.second->m_modifiers.insert(itemPair.second->m_modifiers.end(), extraModifiers.begin(), extraModifiers.end());
	}

	if (itemPair.first)
	{
		ActiveItem *item = static_cast<ActiveItem *>(itemPair.second);
		m_activeItems.push_back(item);
		m_equippedItems[slot] = item;
		AddComponent(item);
	}
	else
	{
		m_equippedItems[slot] = itemPair.second;
		m_passiveItems.push_back(itemPair.second);

		ApplyModifiers(m_equippedItems[slot]->m_modifiers);
	}
}

void Robot::UnequipItem(const size_t slot)
{
	if (nullptr == m_equippedItems[slot])
		return;

	if (m_equippedItems[slot]->GetClassName() == "Item")
	{		
		for (auto it = m_passiveItems.begin(); it != m_passiveItems.end(); ++it)
		{
			if ((*it) == m_equippedItems[slot])
			{
				m_passiveItems.erase(it);
				m_equippedItems[slot] = nullptr;
				return;
			}
		}
	}

	if (m_equippedItems[slot]->GetClassName() == "ActiveItem")
	{
		for (auto it = m_activeItems.begin(); it != m_activeItems.end(); ++it)
		{
			if ((*it) == m_equippedItems[slot])
			{
				m_activeItems.erase(it);

				m_equippedItems[slot]->DestroyRec();
				m_equippedItems[slot] = nullptr;
				return;
			}
		}
	}
}

bool Robot::IsItemAvailable(const std::string itemName) const
{
	ActiveItem *item = FindActive(itemName);

	if (nullptr == item)
	{
		return false;
	}

	return item->m_isAvailable;
}

void Robot::ApplyBuff(Buff buff)
{
	buff.m_expirationTime = Now() + std::chrono::nanoseconds(static_cast<long long>(1000000000.f * (buff.m_duration)));

	PrintUtils::PrintI("[ROBOT][" + m_name + "] Applying buff " + buff.m_name + " for " + PrintUtils::ToString(buff.m_duration) + " seconds.");
	m_activeBuffs.push_back(buff);

	ApplyModifiers(buff.m_modifiers);
}

int Robot::MapDistanceToTarget() const
{
	if (nullptr != m_target)
	{
		return m_mapManager->Distance(GameConstants::ToMapCoords(m_transform.GetWorldPos()), GameConstants::ToMapCoords(m_target->m_transform.GetWorldPos()));
	}

	return 100000000;
}

void Robot::AcquireTarget()
{
	auto allRobots = m_mapManager->GetRobotsInRange(GameConstants::ToMapCoords(m_transform.GetWorldPos()), m_stats.at(StatType::VISION_RANGE).GetValue());

	m_target = nullptr;
	int closestDistance = 1000000;

	for (auto robot : allRobots)
	{
		if (robot->GetID() != this->GetID())
		{
			int dist = m_mapManager->Distance(GameConstants::ToMapCoords(m_transform.GetWorldPos()), GameConstants::ToMapCoords(robot->m_transform.GetWorldPos()));
			if (dist < closestDistance)
			{
				m_target = robot;
				closestDistance = dist;
			}
		}
	}
}

void Robot::ManageBuffs()
{
	std::vector<Buff> newBuffs;

	TimePointNano t = Now();

	for (auto buff : m_activeBuffs)
	{
		if (t < buff.m_expirationTime)
		{
			newBuffs.push_back(buff);
		}
		else
		{
			PrintUtils::PrintI("[ROBOT][" + m_name + "] Removing buff " + buff.m_name);
			RemoveModifiers(buff.m_modifiers);
		}
	}

	m_activeBuffs = newBuffs;

// 	for (auto it = m_activeBuffs.begin(); it != m_activeBuffs.end(); ++it)
// 	{
// 		if (Now() > (*it).m_expirationTime)
// 		{
// 			PrintUtils::PrintI("[ROBOT][" + m_name + "] Removing buff " + (*it).m_name);
// 			RemoveModifiers((*it).m_modifiers);
// 			m_activeBuffs.erase(it);
// 		}
// 	}
}

Item * Robot::FindPassive(const std::string name) const
{
	for (int i = 0; i < m_passiveItems.size(); ++i)
	{
		if (m_passiveItems[i]->m_name == name)
			return m_passiveItems[i];
	}

	return nullptr;
}

ActiveItem * Robot::FindActive(const std::string name) const
{
	if (name == "FireWeapon")
	{
		return m_weapon;
	}

	for (int i = 0; i < m_activeItems.size(); ++i)
	{
		if (m_activeItems[i]->m_name == name)
			return m_activeItems[i];
	}

	return nullptr;
}

void Robot::ApplyModifiers(const std::vector<StatModifier> &mods)
{
	for (auto modifier : mods)
	{
		switch (modifier.m_target)
		{
		default:
			break;
		case ModifierTarget::ROBOT:
			ApplyModifier(modifier);
			break;
		case ModifierTarget::ARMOR:
			m_armor->ApplyModifier(modifier);
			break;
		case ModifierTarget::WEAPON:
			if (nullptr == m_weapon)
				break;
			m_weapon->ApplyModifier(modifier);
			break;
		case ModifierTarget::ITEM:
			///TODO
			break;
		case ModifierTarget::NUM_MODIFIER_TARGET:
			break;
		}
	}
}

void Robot::RemoveModifiers(const std::vector<StatModifier> &mods)
{
	for (auto modifier : mods)
	{
		switch (modifier.m_target)
		{
		default:
			break;
		case ModifierTarget::ROBOT:
			RemoveModifier(modifier);
			break;
		case ModifierTarget::ARMOR:
			m_armor->RemoveModifier(modifier);
			break;
		case ModifierTarget::WEAPON:
			if (nullptr == m_weapon)
				break;
			m_weapon->RemoveModifier(modifier);
			break;
		case ModifierTarget::ITEM:
			///TODO
			break;
		case ModifierTarget::NUM_MODIFIER_TARGET:
			break;
		}
	}
}

