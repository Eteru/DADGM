#include "Graph.h"
#include "PrintUtils.h"

#include <unordered_set>
#include <queue>
#include <functional>
#include <algorithm>
#include <set>

Graph::Graph()
{

}

GraphNode * Graph::NodeAt(Vector2 coords) const
{
	if (!IsInBounds(coords))
		return nullptr;

	return m_nodes[coords.x][coords.y];
}

std::vector<Vector2> Graph::FindPath(Vector2 from, Vector2 to)
{
	if (from == to)
	{
		return std::vector<Vector2>();
	}

	std::unordered_set<GraphNode *> closedSet;
	std::unordered_set<GraphNode *> openSet;

	openSet.insert(NodeAt(from));
	std::unordered_map<GraphNode *, GraphNode *> cameFrom;


	for (int i = 0; i < m_nodes.size(); ++i)
	{
		for (int j = 0; j < m_nodes[i].size(); ++j)
		{
			m_nodes[i][j]->m_gScore = 50000.f;
			m_nodes[i][j]->m_fScore = 50000.f;
		}
	}

	NodeAt(from)->m_gScore = 0;
	NodeAt(from)->m_fScore = Distance(NodeAt(from), NodeAt(to));

	while (!openSet.empty())
	{

		GraphNode *current = *std::min_element(openSet.begin(), openSet.end(), m_cmp);

		if (current == NodeAt(to))
		{
			return ReconstructPath(cameFrom, current);
		}


		closedSet.insert(current);
		openSet.erase(current);




		for (auto outEdge : current->m_outEdges)
		{
			GraphNode *neighbor = outEdge->m_p2;

			if (closedSet.count(neighbor) == 1)
			{
				continue;
			}

			if (openSet.count(neighbor) == 0)
			{
				openSet.insert(neighbor);
			}

			float tentativeGScore = current->m_gScore + outEdge->m_w;
			if (tentativeGScore >= neighbor->m_gScore)
			{
				continue;
			}

			cameFrom[neighbor] = current;
			neighbor->m_gScore = tentativeGScore;
			neighbor->m_fScore = tentativeGScore + Distance(neighbor, NodeAt(to));
		}
	}

	return std::vector<Vector2>();
}

void Graph::InitFromMapString(const Vector2 dims, const std::vector<std::string> mapString)
{

	if (!m_nodes.empty())
	{
		for (int i = 0; i < m_nodes.size(); ++i)
		{
			for (int j = 0; j < m_nodes[i].size(); ++j)
			{
				if (nullptr != m_nodes[i][j])
				{
					delete m_nodes[i][j];
					m_nodes[i][j] = nullptr;
				}
			}
			m_nodes[i].clear();
		}
		m_nodes.clear();
	}



	m_nodes = std::vector<std::vector<GraphNode*>>(dims.x);
	m_dims = dims;

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
				for (auto neighbor : GetAllNeighbors(m_nodes[i][j], mapString))
				{
					if (mapString[neighbor.first->m_mapCoords.x][neighbor.first->m_mapCoords.y] == '0')
					{
						m_nodes[i][j]->m_outEdges.push_back(new DirectedGraphEdge(m_nodes[i][j], neighbor.first, neighbor.second ? 1.41 : 1));
						//m_nodes[i][j]->m_inEdges.push_back(new DirectedGraphEdge(neighbor, m_nodes[i][j], 1));
					}
				}
			}
		}
	}
}

int Graph::Distance(const GraphNode *from, const GraphNode *to)
{
	return static_cast<int>(std::floor(Math::Distance(from->m_mapCoords, to->m_mapCoords)));
}

std::vector<Vector2> Graph::ReconstructPath(std::unordered_map<GraphNode *, GraphNode *> &cameFrom, GraphNode *node)
{
	std::vector<Vector2> result({ node->m_mapCoords });

	GraphNode *crawler = node;

	while (cameFrom.count(crawler))
	{
		crawler = cameFrom[crawler];
		result.push_back(crawler->m_mapCoords);
	}

	std::reverse(result.begin(), result.end());

	return result;
}

std::vector<std::pair<GraphNode *, bool>> Graph::GetAllNeighbors(GraphNode *node, const std::vector<std::string> &mapString) const
{
	Vector2 upRaw = Vector2(0, 1);
	Vector2 downRaw = Vector2(0, -1);
	Vector2 leftRaw = Vector2(-1, 0);
	Vector2 rightRaw = Vector2(1, 0);

	Vector2 up = upRaw + node->m_mapCoords;
	Vector2 down = downRaw + node->m_mapCoords;
	Vector2 left = leftRaw + node->m_mapCoords;
	Vector2 right = rightRaw + node->m_mapCoords;


	std::vector<std::pair<GraphNode *, bool>> result;

	if (IsInBounds(up))
		result.push_back(std::make_pair(NodeAt(up), false));

	if (IsInBounds(down))
		result.push_back(std::make_pair(NodeAt(down), false));

	if (IsInBounds(left))
		result.push_back(std::make_pair(NodeAt(left), false));

	if (IsInBounds(right))
		result.push_back(std::make_pair(NodeAt(right), false));



	Vector2 vec = upRaw + leftRaw + node->m_mapCoords;
	if (CheckAdjacent(up, left, vec, mapString))
		result.push_back(std::make_pair(NodeAt(vec), true));

	vec = upRaw + rightRaw + node->m_mapCoords;
	if (CheckAdjacent(up, right, vec, mapString))
		result.push_back(std::make_pair(NodeAt(vec), true));

	vec = downRaw + leftRaw + node->m_mapCoords;
	if (CheckAdjacent(down, left, vec, mapString))
		result.push_back(std::make_pair(NodeAt(vec), true));

	vec = downRaw + rightRaw + node->m_mapCoords;
	if (CheckAdjacent(down, right, vec, mapString))
		result.push_back(std::make_pair(NodeAt(vec), true));

	return result;
}

bool Graph::CheckAdjacent(Vector2 v1, Vector2 v2, Vector2 adj, const std::vector<std::string> &mapString) const
{
	bool cond = mapString[v1.x][v1.y] == '0' && mapString[v2.x][v2.y] == '0';

	return IsInBounds(v1) && IsInBounds(v2) && cond;
}

bool Graph::IsInBounds(Vector2 coord) const
{
	return !(coord.x < 0 || coord.x >= m_dims.x || coord.y < 0 || coord.y >= m_dims.y);
}

GraphNode::GraphNode(const Vector2 mapCoords)
{
	m_mapCoords = mapCoords;
}

GraphNode::~GraphNode()
{
	// 	for (auto edge : m_inEdges)
	// 	{
	// 		delete edge;
	// 		edge = nullptr;
	// 	}

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
