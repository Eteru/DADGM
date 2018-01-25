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
	m_movementController = new MovementController();
	m_movementController->SetID(UniqueID::GetID(m_movementController->GetClassName()));
	m_movementController->Init();

	AddComponent(m_movementController);
}

void Robot::FixedUpdate()
{
	//PrintUtils::PrintD("Fixed pos: " + m_transform.GetWorldPos().ToString());
// 	if (nullptr != m_physicsBody)
// 	{
// 		m_transform.SetPos(m_physicsBody->m_transform.GetLocalPos());
// 	}

	if (nullptr == m_physicsBody)
	{
		return;
	}

	if (!m_physicsBody->m_hasTarget)
	{
		if (!m_movementController->IsEmpty())
		{
			m_physicsBody->SetTarget(GameConstants::ToWorldCoords(m_movementController->NextPoint(), GameConstants::WALL_HEIGHT));
		}
		else
		{
			auto allFreePoints = m_mapManager->GetAllFreeCells();

			MoveTowards(allFreePoints[std::rand() % allFreePoints.size()]);
		}
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

void Robot::MoveTowards(const Vector2 pos)
{
	if (nullptr == m_mapManager || nullptr == m_physicsBody || nullptr == m_movementController)
	{
		return;
	}

	
	std::vector<Vector2> path = m_mapManager->FindPath(GameConstants::ToMapCoords(m_physicsBody->m_transform.GetWorldPos()), pos);

	if (!path.empty())
	{
		m_movementController->SetPath(path);
	}
}
