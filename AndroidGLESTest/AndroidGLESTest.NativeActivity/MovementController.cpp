#include "MovementController.h"
#include "DebugDrawPrimitives.h"

#include "PhysicsBody.h"
#include "MapManager.h"

MovementController::MovementController(PhysicsBody *physicsBody, MapManager *mapManager)
{
	SetPhysicsBody(physicsBody);
	m_mapManager = mapManager;
}

void MovementController::Init()
{
	m_debugDraw = true;
}

void MovementController::FixedUpdate()
{

	Vector2 mapPos = GameConstants::ToMapCoords(GetPhysicsBody()->m_transform.GetWorldPos());

	SetPath(m_mapManager->FindPath(mapPos, m_target));

	if (IsEmpty())
		return;

	if (mapPos == NextPoint())
	{
		ArrivedAtCheckpoint();
	}

	m_physicsBody->SetTarget(GameConstants::ToWorldCoords(NextPoint(), GameConstants::WALL_HEIGHT));
}

void MovementController::Update()
{
}

void MovementController::Draw()
{
}

void MovementController::DebugDraw()
{
	if (m_path.empty())
		return;

	for (int i = 0; i < m_path.size() - 1; ++i)
	{		
		DebugDrawPrimitives::DrawLine(GameConstants::ToWorldCoords(m_path[i], GameConstants::WALL_HEIGHT), GameConstants::ToWorldCoords(m_path[i + 1], GameConstants::WALL_HEIGHT), DebugDrawPrimitives::COLOR_BLUE);
	}

	DebugDrawPrimitives::DrawSphere(GameConstants::ToWorldCoords(m_path.back(), GameConstants::WALL_HEIGHT), 0.5f, DebugDrawPrimitives::COLOR_BLUE);
}

void MovementController::Destroy()
{
}

std::string MovementController::ToString()
{
	return std::string("TODO MovementController string");
}

std::string MovementController::GetClassName()
{
	return std::string("MovementController");
}

void MovementController::SetTarget(Vector2 target)
{
	m_target = target;
}

void MovementController::SetRandomTarget()
{
	auto allFreePoints = m_mapManager->GetAllFreeCells();
	m_target = allFreePoints[std::rand() % allFreePoints.size()];
}

void MovementController::SetPath(std::vector<Vector2> path)
{
	m_path = path;
}

Vector2 MovementController::NextPoint()
{
	return m_path.front();
}

void MovementController::ArrivedAtCheckpoint()
{
	m_path.erase(m_path.begin());
}

bool MovementController::IsEmpty()
{
	return m_path.empty();
}
