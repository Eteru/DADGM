#include "Graph.h"

Graph::Graph()
{

}

void Graph::InitFromMapString(const Vector2 dims, const std::vector<std::string> mapString)
{
	m_nodes = std::vector<std::vector<GraphNode*>>(dims.x);

	for (int i = 0; i < m_nodes.size(); ++i)
	{
		m_nodes[i] = std::vector<GraphNode *>(dims.y);

		for (int j = 0; j < m_nodes[i].size(); ++j)
		{
			m_nodes[i][j] = new GraphNode(Vector2(i, j));
		}
	}

	for (int i = 0; i < m_nodes.size(); ++i)
	{
		for (int j = 0; j < m_nodes[i].size(); ++j)
		{
			if (mapString[i][j] == '0')
			{
				for (auto neighbor : GetAllNeighbors(m_nodes[i][j]))
				{
					if (mapString[neighbor->m_mapCoords.x][neighbor->m_mapCoords.y] == '0')
					{
						m_nodes[i][j]->m_outEdges.push_back(new DirectedGraphEdge(m_nodes[i][j], neighbor, 1));
						m_nodes[i][j]->m_inEdges.push_back(new DirectedGraphEdge(neighbor, m_nodes[i][j], 1));
					}
				}
			}
		}
	}
}

int Graph::Distance(const GraphNode *from, const GraphNode *to)
{
	return static_cast<int>(std::ceil(Math::Distance(from->m_mapCoords, to->m_mapCoords)));
}

std::vector<GraphNode *> Graph::GetAllNeighbors(GraphNode *node) const
{
	std::vector<Vector2> rawCoords({ Vector2(-1, -1) , Vector2(-1, 0), Vector2(-1, 1), Vector2(0, -1) , Vector2(0, 1), Vector2(1, -1) , Vector2(1, 0), Vector2(1, 1) });

	for (int i = 0; i < rawCoords.size(); ++i)
	{
		rawCoords[i] += node->m_mapCoords;
	}


	std::vector<GraphNode *> result;

	for (int i = 0; i < rawCoords.size(); ++i)
	{
		if (IsInBounds(rawCoords[i]))
		{
			result.push_back(m_nodes[rawCoords[i].x][rawCoords[i].y]);
		}
	}

	return result;
}

bool Graph::IsInBounds(const Vector2 coord) const
{
	return !(coord.x < 0 || coord.x >= m_dims.x || coord.y < 0 || coord.y >= m_dims.y);
}

GraphNode::GraphNode(const Vector2 mapCoords)
{
	m_mapCoords = mapCoords;
}

GraphNode::~GraphNode()
{
	for (auto edge : m_inEdges)
	{
		delete edge;
		edge = nullptr;
	}

	for (auto edge : m_outEdges)
	{
		delete edge;
		edge = nullptr;
	}
}

DirectedGraphEdge::DirectedGraphEdge(GraphNode *from, GraphNode *to, const float weight)
{
	m_p1 = from;
	m_p2 = to;
	m_w = weight;
}
