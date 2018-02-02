#include "Robot.h"
#include "SceneObjectSpawner.h"
#include "UniqueID.h"
#include "BoundingSphere.h"

#include "PhysicsBody.h"
#include "MapManager.h"
#include "MovementController.h"

Robot::Robot()
	: m_armor(nullptr), m_weapon(nullptr)
{

}

void Robot::Init()
{
	
}

void Robot::FixedUpdate()
{
	if (nullptr != m_physicsBody)
	{
		m_physicsBody->m_acceleration = m_armor->m_stats.at(StatType::LINEAR_ACCEL).GetValue();
		m_physicsBody->m_topSpeed = m_armor->m_stats.at(StatType::LINEAR_TOP).GetValue();
		m_physicsBody->m_turningAcceleration = m_armor->m_stats.at(StatType::ANGULAR_ACCEL).GetValue();
		m_physicsBody->m_topTurningSpeed = m_armor->m_stats.at(StatType::ANGULAR_TOP).GetValue();
	}
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

int Robot::MapDistanceToTarget() const
{
	if (nullptr != m_target)
	{
		return static_cast<int>(round(Math::Distance(m_transform.GetWorldPos(), m_target->m_transform.GetWorldPos()) / GameConstants::CELL_SIZE));
	}

	return 100000000;
}

void Robot::AcquireTarget()
{

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
