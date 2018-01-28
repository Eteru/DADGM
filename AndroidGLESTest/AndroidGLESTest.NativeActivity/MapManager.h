#pragma once

#include "GameLoopObject.h"
#include "Graph.h"

class MapManager : public GameLoopObject
{

public:
	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Destroy() override;

	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	virtual void Draw() override;

	std::vector<Vector2> FindPath(Vector2 from, Vector2 to);
	std::vector<Vector2> GetAllFreeCells();
	std::vector<Vector2> GetCellsOnCircle(const Vector2 center, const int radius) const;

private:
	static int Distance(const Vector2 from, const Vector2 to);
	void SpawnFromString(Vector2 dims, std::vector<std::string> &string);


	Graph m_graph;
	Vector2 m_mapDims;
	std::vector<std::string> m_mapString;
};

