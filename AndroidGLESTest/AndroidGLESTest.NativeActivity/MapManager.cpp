#include "MapManager.h"
#include "SceneObjectSpawner.h"

#include "SceneManager.h"
#include "DebugDrawPrimitives.h"



Vector2 MapManager::GetRandomSpawnPoint()
{
	return m_spawnPoints[rand() % m_spawnPoints.size()];
}

std::pair<Vector2, Vector2> MapManager::GetRandomOptimalSpawns()
{
	Vector2 firstSpawn = GetRandomSpawnPoint();
	Vector2 secondSpawn = m_spawnPoints[0];

	int maxDistance = Distance(firstSpawn, secondSpawn);

	for (int i = 1; i < m_spawnPoints.size(); ++i)
	{
		int dist = Distance(firstSpawn, m_spawnPoints[i]);
		if (dist > maxDistance)
		{
			secondSpawn = m_spawnPoints[i];
			maxDistance = dist;
		}
	}

	return std::make_pair(firstSpawn, secondSpawn);
}

void MapManager::InitFromDesc(const MapDescription desc)
{
	m_mapDims = desc.m_dims;
	m_mapString = desc.m_mapString;
	m_spawnPoints = desc.m_spawnPoints;

	for (auto it = m_spawnPoints.begin(); it != m_spawnPoints.end(); ++it)
	{
		if (m_mapString[(*it).x][(*it).y] != '0')
			m_spawnPoints.erase(it);
	}


	SpawnFromString(m_mapDims, m_mapString);
	m_graph.InitFromMapString(m_mapDims, m_mapString);
}

void MapManager::Init()
{

// 	auto pr = SceneManager::GetInstance()->LoadMapFromFile("XMLs/adevarataHarta.xml");
// 
// 	m_mapDims = pr.first;
// 	m_mapString = pr.second;
// 
// 	SpawnFromString(m_mapDims, m_mapString);
// 
// 	m_graph.InitFromMapString(m_mapDims, m_mapString);
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

std::vector<Vector2> MapManager::GetAllFreeCells()
{
	std::vector<Vector2> result;

	for (int i = 0; i < m_mapString.size(); ++i)
	{
		for (int j = 0; j < m_mapString[i].size(); ++j)
		{
			if (m_mapString[i][j] == '0')
			{
				result.push_back(Vector2(i, j));
			}
		}
	}

	return result;
}

std::vector<Vector2> MapManager::GetCellsOnCircle(const Vector2 center, const int radius) const
{
	std::vector<Vector2> result;

	for (int i = 0; i < m_mapString.size(); ++i)
	{
		for (int j = 0; j < m_mapString[i].size(); ++j)
		{
			if (m_mapString[i][j] == '0' && Distance(center, Vector2(i, j)) == radius)
			{
				result.push_back(Vector2(i, j));
			}
		}
	}

	return result;
}

int MapManager::Distance(const Vector2 from, const Vector2 to)
{
	return static_cast<int>(std::floor(Math::Distance(from, to)));
}

void MapManager::SpawnFromString(Vector2 dims, std::vector<std::string> &string)
{
	DestroyComponents("MapCell");

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

