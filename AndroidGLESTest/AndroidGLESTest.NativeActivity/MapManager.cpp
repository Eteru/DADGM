#include "MapManager.h"
#include "SceneObjectSpawner.h"

#include "SceneManager.h"



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

void MapManager::SpawnFromString(Vector2 dims, std::vector<std::string> &string)
{

	for (int i = 0; i < dims.x; ++i)
	{
		for (int j = 0; j < dims.y; ++j)
		{

			MapCell *cell;
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

