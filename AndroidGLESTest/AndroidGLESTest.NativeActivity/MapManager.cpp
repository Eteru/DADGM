#include "MapManager.h"
#include "SceneObjectSpawner.h"

#include "SceneManager.h"
#include "DebugDrawPrimitives.h"



void MapManager::Init()
{

	auto pr = SceneManager::GetInstance()->LoadMapFromFile("XMLs/adevarataHarta.xml");

	m_mapDims = pr.first;
	m_mapString = pr.second;

	SpawnFromString(m_mapDims, m_mapString);

	m_graph.InitFromMapString(m_mapDims, m_mapString);
}

void MapManager::FixedUpdate()
{

	
}

void MapManager::Destroy()
{
}

std::string MapManager::ToString()
{
	return std::string("TODO MapManager string");
}

std::string MapManager::GetClassName()
{
	return std::string("MapManager");
}

void MapManager::Draw()
{
// 	for (auto nodeRow : m_graph.m_nodes)
// 	{
// 		for (auto node : nodeRow)
// 		{
// 			for (auto outEdge : node->m_outEdges)
// 			{
// 				DebugDrawPrimitives::DrawLine(Vector3(outEdge->m_p1->m_mapCoords.x, 2 * GameConstants::WALL_HEIGHT, outEdge->m_p1->m_mapCoords.y), Vector3(outEdge->m_p2->m_mapCoords.x, 2 * GameConstants::WALL_HEIGHT, outEdge->m_p2->m_mapCoords.y), DebugDrawPrimitives::COLOR_RED);
// 			}
// 		}
// 	}
}

std::vector<Vector2> MapManager::FindPath(Vector2 from, Vector2 to)
{
	if (m_mapString[from.x][from.y] != '0' || m_mapString[to.x][to.y] != '0')
	{
		return std::vector<Vector2>();
	}
	return m_graph.FindPath(from, to);
}

void MapManager::SpawnFromString(Vector2 dims, std::vector<std::string> &string)
{

	for (int i = 0; i < dims.x; ++i)
	{
		for (int j = 0; j < dims.y; ++j)
		{

			MapCell *cell = nullptr;
			if ('0' == string[i][j])
			{
				cell = SceneObjectSpawner::SpawnMapCell(Vector2(i, j), SceneObjectSpawner::MapObjectType::GROUND);
			}
			else if ('1' == string[i][j])
			{
				cell = SceneObjectSpawner::SpawnMapCell(Vector2(i, j), SceneObjectSpawner::MapObjectType::WALL);
			}

			if (nullptr != cell)
			{
				AddComponent(cell);
			}
		}
	}
}

