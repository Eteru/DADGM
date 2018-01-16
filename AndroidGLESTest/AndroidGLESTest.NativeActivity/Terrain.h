#pragma once

#include "Vertex.h"
#include "SceneObject.h"

class Terrain :
	public SceneObject
{
public:
	Terrain(Vector3 pos, Vector3 rot, Vector3 scale, Vector3 heights, bool depth_test, std::string name);
	~Terrain();

	inline void SetOffsetY(float offsetY)
	{
		m_offsetY = offsetY;
	}

	inline void SetCellSize(uint32_t cell_size)
	{
		m_cell_size = cell_size;
	}

	inline void SetBlockSize(uint32_t block_size)
	{
		m_block_size = block_size;
	}

	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Destroy() override;

	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

protected:
	float m_offsetY;
	uint32_t m_half_size;
	uint32_t m_cell_size;
	uint32_t m_block_size;

	Vector3 m_heights;
	std::vector<Vertex> m_vertices;
};

