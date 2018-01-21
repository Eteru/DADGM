#include "MapManager.h"
#include "SceneObjectSpawner.h"

void MapManager::Init()
{
	int testMax = 15;
	for (int i = 0; i < testMax; ++i)
	{
		for (int j = 0; j < testMax; ++j)
		{
			MapCell *cell = SceneObjectSpawner::SpawnMapCell(Vector2(i, j), i == 0 || j == 0 || i == testMax - 1 || j == testMax - 1 ? SceneObjectSpawner::MapObjectType::WALL : SceneObjectSpawner::MapObjectType::GROUND);
			AddComponent(cell);
		}
	}
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

