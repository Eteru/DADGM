#pragma once
#include <vector>
#include "Math.h"

class DirectedGraphEdge;

class GraphNode
{
public:	
	GraphNode(const Vector2 mapCoords);
	~GraphNode();

	Vector2 m_mapCoords;
	std::vector<DirectedGraphEdge *> m_outEdges;
	std::vector<DirectedGraphEdge *> m_inEdges;
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

	void InitFromMapString(const Vector2 dims, const std::vector<std::string> mapString);

	static int Distance(const GraphNode *from, const GraphNode *to);
private:

	std::vector<GraphNode *> GetAllNeighbors(GraphNode *node) const;
	bool IsInBounds(const Vector2 coord) const;
};