#include "MovementController.h"
#include "DebugDrawPrimitives.h"

void MovementController::Init()
{
	m_debugDraw = true;
}

void MovementController::FixedUpdate()
{
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

void MovementController::SetPath(std::vector<Vector2> path)
{
	m_path = path;
}

Vector2 MovementController::NextPoint()
{
	Vector2 result = m_path.front();
	m_path.erase(m_path.begin());
	return result;
}

void MovementController::ArrivedAtCheckpoint()
{

}

bool MovementController::IsEmpty()
{
	return m_path.empty();
}
