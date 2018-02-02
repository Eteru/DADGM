#include "AIActions.h"

#include "MovementController.h"
#include "MapManager.h"
#include "PhysicsBody.h"
#include "DebugDrawPrimitives.h"

void AIMovementActions::SetDestination(MovementActionType type, MovementController *movementController, GameLoopObject *target, MapManager *mapManager)
{
	switch (type)
	{
	case MovementActionType::STAND_STILL:		
		break;
	case MovementActionType::EXPLORE:
		AIMovementActions::Explore(movementController, mapManager);
		break;
	case MovementActionType::KEEP_DISTANCE:
		AIMovementActions::KeepDistance(movementController, mapManager, target, 5.f);
		break;
	case MovementActionType::RUN_AWAY:
		AIMovementActions::RunAway(movementController, mapManager, target);
		break;
	case MovementActionType::NUM_MOVEMENT_ACTION_TYPE:
		break;
	default:
		break;
	}
}

void AIMovementActions::Explore(MovementController *movementController, MapManager *mapManager)
{
	if (movementController->IsEmpty())
	{
		auto allFreePoints = mapManager->GetAllFreeCells();
		movementController->SetTarget(allFreePoints[std::rand() % allFreePoints.size()]);
	}
}

void AIMovementActions::KeepDistance(MovementController *movementController, MapManager *mapManager, GameLoopObject *target, int distance)
{
	Vector2 robotMapPos = GameConstants::ToMapCoords(movementController->GetPhysicsBody()->m_transform.GetWorldPos());
	Vector2 targetMapPos = GameConstants::ToMapCoords(target->m_transform.GetWorldPos());

	if (MapManager::Distance(robotMapPos, targetMapPos) == distance)
	{
		return;
	}

	auto allPoints = mapManager->GetCellsOnCircle(targetMapPos, distance);

	if (!allPoints.empty())
	{
		Vector2 closestPoint = allPoints[0];

		for (auto pt : allPoints)
		{
			if (MapManager::Distance(robotMapPos, pt) < MapManager::Distance(robotMapPos, closestPoint))
			{
				closestPoint = pt;
			}
		}

		//m_target = closestPoint;
		movementController->SetTarget(closestPoint);
	}
}

void AIMovementActions::RunAway(MovementController *movementController, MapManager *mapManager, GameLoopObject *target)
{
	Vector2 robotMapPos = GameConstants::ToMapCoords(movementController->GetPhysicsBody()->m_transform.GetWorldPos());
	Vector2 targetMapPos = GameConstants::ToMapCoords(target->m_transform.GetWorldPos());	
	

	auto allPoints = mapManager->GetCellsOnCircle(targetMapPos, MapManager::Distance(robotMapPos, targetMapPos) + 1);

	if (!allPoints.empty())
	{
		Vector2 closestPoint = allPoints[0];

		for (auto pt : allPoints)
		{
			if (MapManager::Distance(robotMapPos, pt) < MapManager::Distance(robotMapPos, closestPoint))
			{
				closestPoint = pt;
			}
		}

		//m_target = closestPoint;
		movementController->SetTarget(closestPoint);
	}
}



// std::string AIAction::ToString()
// {
// 	return std::string("TODO AIAction string");
// }
// 
// std::string AIAction::GetClassName()
// {
// 	return std::string("AIAction");
// }
// 
// MovementAction::MovementAction(MovementController *movementController, MapManager *mapManager)
// {
// 	m_movementController = movementController;
// 	m_mapManager = mapManager;
// }
// 
// MovementAction::MovementAction * MovementAction::GetAction(const MovementActionType type)
// {
// 
// }
// 
// bool MovementAction::IsFinished()
// {
// 	return m_movementController->IsEmpty();
// }
// 
// std::string MovementAction::ToString()
// {
// 	return std::string("TODO MovementAction string");
// }
// 
// std::string MovementAction::GetClassName()
// {
// 	return std::string("MovementAction");
// }

// Explore::Explore(MovementController *movementController, MapManager *mapManager) : MovementAction(movementController, mapManager)
// {
// 
// }
// 
// std::string Explore::ToString()
// {
// 	return std::string("TODO Explore string");
// }
// 
// std::string Explore::GetClassName()
// {
// 	return std::string("Explore");
// }
// 
// void Explore::FixedUpdate()
// {
// 	if (IsFinished())
// 	{
// 		auto allFreePoints = m_mapManager->GetAllFreeCells();
// 		m_target = allFreePoints[std::rand() % allFreePoints.size()];
// 	}
// 
// 	m_movementController->SetTarget(m_target);
// }
// 
// KeepDistance::KeepDistance(const Vector2 target, int distance, MovementController *movementController, MapManager *mapManager) : MovementAction(movementController, mapManager)
// {
// 	m_distance = distance;
// 	m_target = target;
// }
// 
// std::string KeepDistance::ToString()
// {
// 	return std::string("TODO KeepDistance string");
// }
// 
// std::string KeepDistance::GetClassName()
// {
// 	return std::string("KeepDistance");
// }
// 
// void KeepDistance::DebugDraw()
// {
// 	auto allFreePoints = m_mapManager->GetCellsOnCircle(m_target, m_distance);
// 
// 	DebugDrawPrimitives::DrawSphere(GameConstants::ToWorldCoords(m_target, GameConstants::WALL_HEIGHT), 0.25f, Vector4(0.f, 1.f, 1.f, 1.f));
// 	for (auto pt : allFreePoints)
// 	{
// 		DebugDrawPrimitives::DrawSphere(GameConstants::ToWorldCoords(pt, GameConstants::WALL_HEIGHT), 0.25f, DebugDrawPrimitives::COLOR_YELLOW);
// 	}
// }
// 
// void KeepDistance::FixedUpdate()
// {
// 	UpdateClosestPoint();
// 	m_movementController->SetTarget(m_closestPoint);
// }
// 
// void KeepDistance::UpdateClosestPoint()
// {
// 	Vector2 robotMapPos = GameConstants::ToMapCoords(m_movementController->GetPhysicsBody()->m_transform.GetWorldPos());
// 	if (Math::Distance(robotMapPos, m_target) == m_distance)
// 	{
// 		return;
// 	}
// 
// 	m_allPoints = m_mapManager->GetCellsOnCircle(m_target, m_distance);
// 
// 	if (!m_allPoints.empty())
// 	{
// 		Vector2 closestPoint = m_allPoints[0];
// 
// 		for (auto pt : m_allPoints)
// 		{
// 			if (Math::Distance(robotMapPos, pt) < Math::Distance(robotMapPos, closestPoint))
// 			{
// 				closestPoint = pt;
// 			}
// 		}
// 
// 		//m_target = closestPoint;
// 		m_closestPoint = closestPoint;
// 
// 	}
// }
// 
// std::string ItemAction::ToString()
// {
// 	return std::string("TODO ItemAction string");
// }
// 
// std::string ItemAction::GetClassName()
// {
// 	return std::string("ItemAction");
// }
// 
// std::string StandStill::ToString()
// {
// 	return std::string("TODO StandStill string");
// }
// 
// std::string StandStill::GetClassName()
// {
// 	return std::string("StandStill");
// }
// 
// void StandStill::FixedUpdate()
// {
// }
// 
// RunAway::RunAway(const Vector2 target, MovementController *movementController, MapManager *mapManager) : MovementAction(movementController, mapManager)
// {
// 	m_target = target;
// 	m_movementController = movementController;
// 	m_mapManager = m_mapManager;
// }
// 
// std::string RunAway::ToString()
// {
// 	return std::string("TODO RunAway string");
// }
// 
// std::string RunAway::GetClassName()
// {
// 	return std::string("RunAway");
// }
// 
// void RunAway::FixedUpdate()
// {
// }

