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
private:
	void SpawnFromString(Vector2 dims, std::vector<std::string> &string);


	Graph m_graph;
	Vector2 m_mapDims;
	std::vector<std::string> m_mapString;
};

