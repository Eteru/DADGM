
#include "Model.h"
#include "SceneManager.h"
#include <android/log.h>

#include "PrintUtils.h"
#include "obj_loader.h"

Model::Model() : m_loaded(false), m_mr(nullptr)
{
}

Model::Model(ModelResource *mr) : m_loaded(false), m_mr(mr)
{
}

Model::~Model()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_iboID);
	glDeleteBuffers(1, &m_wiredIboID);
}

bool Model::Load()
{
	if (true == m_loaded) {
		return true;
	}

	if (nullptr == m_mr) {
		return false;
	}

	LOGD("filepath: %s\n", m_mr->model_path.c_str());

	IndexedModel model = OBJModel(m_mr->model_path).ToIndexedModel();
	
	int pos = 0;
	int crt_pos = 0;

	// Read vertices
	m_pos_min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_pos_max = Vector3(FLT_MIN, FLT_MIN, FLT_MIN);
	std::vector<Vertex> vertices;
	for (int i = 0; i < model.positions.size(); ++i) {
		Vertex v;
		v.pos = model.positions[i];
		v.normal = model.normals[i];
		v.uv = model.texCoords[i];

		m_pos_min.x = std::min(m_pos_min.x, v.pos.x);
		m_pos_min.y = std::min(m_pos_min.y, v.pos.y);
		m_pos_min.z = std::min(m_pos_min.z, v.pos.z);

		m_pos_max.x = std::max(m_pos_max.x, v.pos.x);
		m_pos_max.y = std::max(m_pos_max.y, v.pos.y);
		m_pos_max.z = std::max(m_pos_max.z, v.pos.z);

		v.color = Vector3(1, 1, 1);
		v.uv_blend = Vector2(1, 1);

		vertices.push_back(v);
	}

	// Generate and create vbo
	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Generate and create ibo for filled mesh
	m_indicesCount = model.indices.size();
	glGenBuffers(1, &m_iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * m_indicesCount, &model.indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Create ibo buffer for wired mesh
	m_indicesWiredCount = m_indicesCount * 2;
	std::vector<GLushort> indicesWired(m_indicesWiredCount);
	for (int i = 0, wiredCount = 0; i < m_indicesCount; i += 3, wiredCount += 6) {
		indicesWired[wiredCount] = model.indices[i];
		indicesWired[wiredCount + 1] = model.indices[i + 1];
		indicesWired[wiredCount + 2] = model.indices[i + 1];
		indicesWired[wiredCount + 3] = model.indices[i + 2];
		indicesWired[wiredCount + 4] = model.indices[i + 2];
		indicesWired[wiredCount + 5] = model.indices[i];
	}

	// Generate and create ibo for wired mesh
	glGenBuffers(1, &m_wiredIboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_wiredIboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * m_indicesWiredCount, &indicesWired[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_loaded = true;
	return true;
}

std::vector<Vertex> Model::GenerateFlatModel(uint32_t blockSize, uint32_t cellSize, float offsetY)
{
	uint32_t line_size = blockSize + 1;
	size_t verts_count = line_size * line_size;
	std::vector<Vertex> vertices(verts_count);
	std::vector<GLushort> indices;

	for (int i = 0; i < line_size; ++i) {
		for (int j = 0; j < line_size; ++j) {
			size_t idx = i * line_size + j;
			vertices[idx].pos = Vector3(j * cellSize, offsetY, i * cellSize);
			vertices[idx].normal = Vector3(0, 1, 0);
			vertices[idx].color = Vector3(1, 1, 1);
			vertices[idx].uv = Vector2(i, j);
			vertices[idx].uv_blend = Vector2(static_cast<float>(i) / line_size, static_cast<float>(j) / line_size);

			if (i < blockSize && j < blockSize) {
				indices.push_back(idx);
				indices.push_back(idx + line_size + 1);
				indices.push_back(idx + line_size);
				indices.push_back(idx);
				indices.push_back(idx + 1);
				indices.push_back(idx + line_size + 1);
			}
		}
	}

	// Generate and create vbo
	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verts_count, &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_indicesCount = indices.size();
	// Generate and create ibo for filled mesh
	glGenBuffers(1, &m_iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indicesCount, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_loaded = true;

	return vertices;
}

void Model::RebindBuffer(std::vector<Vertex>& vertices)
{
	for (Vertex & v : vertices) {
		m_pos_min.x = std::min(m_pos_min.x, v.pos.x);
		m_pos_min.y = std::min(m_pos_min.y, v.pos.y);
		m_pos_min.z = std::min(m_pos_min.z, v.pos.z);

		m_pos_max.x = std::max(m_pos_max.x, v.pos.x);
		m_pos_max.y = std::max(m_pos_max.y, v.pos.y);
		m_pos_max.z = std::max(m_pos_max.z, v.pos.z);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
