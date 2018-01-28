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
	if (nullptr != m_armor)
	{
		delete m_armor;
		m_armor = nullptr;
	}

	if (nullptr != m_weapon)
	{
		delete m_weapon;
		m_weapon = nullptr;
	}
}

std::string Robot::ToString()
{
	return std::string("TODO Robot string");
}

std::string Robot::GetClassName()
{
	return std::string("Robot");
}