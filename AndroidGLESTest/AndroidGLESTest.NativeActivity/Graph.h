#pragma once
#include <vector>
#include "Math.h"
#include <unordered_map>

class DirectedGraphEdge;

class GraphNode
{
public:	
	GraphNode(const Vector2 mapCoords);
	~GraphNode();

	Vector2 m_mapCoords;
	std::vector<DirectedGraphEdge *> m_outEdges;	

	float m_fScore;
	float m_gScore;
};

class DirectedGraphEdge
{
public:
	DirectedGraphEdge(GraphNode *from, GraphNode *to, const float weight);
	GraphNode *m_p1;
	GraphNode *m_p2;
	float m_w;
};

class Graph
{
public:
	std::vector<std::vector<GraphNode *>> m_nodes;
	Vector2 m_dims;
	Graph();

	GraphNode *NodeAt(Vector2 coords) const;
	std::vector<Vector2> FindPath(Vector2 from, Vector2 to);
	void InitFromMapString(const Vector2 dims, const std::vector<std::string> mapString);

	static int Distance(const GraphNode *from, const GraphNode *to);

private:

	std::vector<Vector2> ReconstructPath(std::unordered_map<GraphNode *, GraphNode *> &cameFrom, GraphNode *node);

	struct Compare
	{
	public:
		bool operator() (const GraphNode *lhs, const GraphNode *rhs) const
		{
			return lhs->m_fScore < rhs->m_fScore;
		}
	} m_cmp;

	std::vector<std::pair<GraphNode *, bool>> GetAllNeighbors(GraphNode *node, const std::vector<std::string> &mapString) const;

	bool CheckAdjacent(Vector2 v1, Vector2 v2, Vector2 adj, const std::vector<std::string> &mapString) const;
	bool IsInBounds(Vector2 coord) const;

	std::unordered_map<GraphNode *, float> m_fScore;
};