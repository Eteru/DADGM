#pragma once

#include <cinttypes>
#include <string>
#include <vector>
#include "Vertex.h"
#include "Structs.h"
#include "BoundingBox.h"

class Model
{
public:
	Model();
	Model(ModelResource *mr);

	~Model();

	bool Load();
	std::vector<Vertex> GenerateFlatModel(uint32_t blockSize, uint32_t cellSize, float offsetY);
	void RebindBuffer(std::vector<Vertex> & vertices);

	inline bool IsLoaded()
	{
		return m_loaded;
	}

	inline GLuint GetVBO() const
	{
		return m_vboID;
	}

	inline GLuint GetIBO(bool is_wired) const
	{
		return (true == is_wired) ? m_wiredIboID : m_iboID;
	}

	inline uint32_t GetIBOCount(bool is_wired) const
	{
		return (true == is_wired) ? m_indicesWiredCount : m_indicesCount;
	}

	inline const BoundingBox & GetBB() const
	{
		return m_bb;
	}

private:
	bool m_loaded;
	GLuint m_iboID;
	GLuint m_wiredIboID;
	GLuint m_vboID;
	uint32_t m_indicesCount;
	uint32_t m_indicesWiredCount;

	BoundingBox m_bb;
	ModelResource *m_mr;
};