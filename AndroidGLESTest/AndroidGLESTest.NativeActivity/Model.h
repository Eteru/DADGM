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

	inline const Vector3 & GetMinPos() const
	{
		return m_pos_min;
	}

	inline const Vector3 & GetMaxPos() const
	{
		return m_pos_max;
	}

private:
	bool m_loaded;
	GLuint m_iboID;
	GLuint m_wiredIboID;
	GLuint m_vboID;
	uint32_t m_indicesCount;
	uint32_t m_indicesWiredCount;

	Vector3 m_pos_min;
	Vector3 m_pos_max;
	
	ModelResource *m_mr;

	void CalcNormals(std::vector<Vertex> & verts, std::vector<GLushort> & indices);
};