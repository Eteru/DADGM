#include "AIActions.h"

#include "MovementController.h"
#include "MapManager.h"
#include "PhysicsBody.h"
#include "DebugDrawPrimitives.h"

std::string AIAction::ToString()
{
	return std::string("TODO AIAction string");
}

std::string AIAction::GetClassName()
{
	return std::string("AIAction");
}

MovementAction::MovementAction(MovementController *movementController, MapManager *mapManager)
{
	m_movementController = movementController;
	m_mapManager = mapManager;
}

bool MovementAction::IsFinished()
{
	return m_movementController->IsEmpty();
}

std::string MovementAction::ToString()
{
	return std::string("TODO MovementAction string");
}

std::string MovementAction::GetClassName()
{
	return std::string("MovementAction");
}

Explore::Explore(MovementController *movementController, MapManager *mapManager) : MovementAction(movementController, mapManager)
{

}

std::string Explore::ToString()
{
	return std::string("TODO Explore string");
}

std::string Explore::GetClassName()
{
	return std::string("Explore");
}

void Explore::FixedUpdate()
{
	if (IsFinished())
	{
		auto allFreePoints = m_mapManager->GetAllFreeCells();
		m_target = allFreePoints[std::rand() % allFreePoints.size()];
	}

	m_movementController->SetTarget(m_target);
}

KeepDistance::KeepDistance(const Vector2 target, int distance, MovementController *movementController, MapManager *mapManager) : MovementAction(movementController, mapManager)
{
	m_distance = distance;
	m_target = target;
}

std::string KeepDistance::ToString()
{
	return std::string("TODO KeepDistance string");
}

std::string KeepDistance::GetClassName()
{
	return std::string("KeepDistance");
}

void KeepDistance::DebugDraw()
{
	auto allFreePoints = m_mapManager->GetCellsOnCircle(m_target, m_distance);

	DebugDrawPrimitives::DrawSphere(GameConstants::ToWorldCoords(m_target, GameConstants::WALL_HEIGHT), 0.25f, Vector4(0.f, 1.f, 1.f, 1.f));
	for (auto pt : allFreePoints)
	{
		DebugDrawPrimitives::DrawSphere(GameConstants::ToWorldCoords(pt, GameConstants::WALL_HEIGHT), 0.25f, DebugDrawPrimitives::COLOR_YELLOW);
	}
}

void KeepDistance::FixedUpdate()
{
	UpdateClosestPoint();
	m_movementController->SetTarget(m_closestPoint);
}

void KeepDistance::UpdateClosestPoint()
{
	Vector2 robotMapPos = GameConstants::ToMapCoords(m_movementController->GetPhysicsBody()->m_transform.GetWorldPos());
	if (Math::Distance(robotMapPos, m_target) == m_distance)
	{
		return;
	}

	m_allPoints = m_mapManager->GetCellsOnCircle(m_target, m_distance);

	if (!m_allPoints.empty())
	{
		Vector2 closestPoint = m_allPoints[0];

		for (auto pt : m_allPoints)
		{
			if (Math::Distance(robotMapPos, pt) < Math::Distance(robotMapPos, closestPoint))
			{
				closestPoint = pt;
			}
		}

		//m_target = closestPoint;
		m_closestPoint = closestPoint;

	}
}

std::string ItemAction::ToString()
{
	return std::string("TODO ItemAction string");
}

std::string ItemAction::GetClassName()
{
	return std::string("ItemAction");
}
