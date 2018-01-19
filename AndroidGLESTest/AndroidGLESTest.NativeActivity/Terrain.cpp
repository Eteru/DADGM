
#include "Terrain.h"
#include "Vertex.h"
#include "SceneManager.h"

Terrain::Terrain(Vector3 pos, Vector3 rot, Vector3 scale, Vector3 heights, bool depth_test, std::string name)
	: SceneObject(pos, rot, scale, name, depth_test)
{
	m_transform = Transform(pos, rot, scale);
	m_transform.m_lerp = false;
	m_heights = heights;
}

Terrain::~Terrain()
{ // Terrains has its own model that has to be deleted
	if (nullptr != m_model)
		delete m_model;
}

void Terrain::Init()
{
	m_model = new Model();
	m_vertices = m_model->GenerateFlatModel(m_block_size, m_cell_size, m_offsetY);
	m_half_size = m_block_size * m_cell_size / 2.f;

	// Center terrain to camera position
	Camera *cam = SceneManager::GetInstance()->GetActiveCamera();

	PrintUtils::PrintI("Active camera: " + cam->ToString());
	m_transform.m_pos.x = cam->m_transform.GetPosition().x - m_half_size;
	m_transform.m_pos.z = cam->m_transform.GetPosition().z - m_half_size;
}

void Terrain::Update()	
{
	/// TODO This is ugly. Transforms should only be modified in a fixed update.
	/// Luckily we won't need this kind of terrain

	Camera *cam = SceneManager::GetInstance()->GetActiveCamera();
	Vector3 camera_pos = cam->m_transform.GetPosition();

	float d_X = camera_pos.x - (m_transform.GetPosition().x + m_half_size);
	float d_Z = camera_pos.z - (m_transform.GetPosition().z + m_half_size);
	float d_Value_X = 0, d_Value_Y = 0;

	if (d_X >= m_cell_size) {
		m_transform.m_pos.x += m_cell_size;
		d_Value_Y = 1.f / m_block_size;
	}
	else if (-d_X >= m_cell_size) {
		m_transform.m_pos.x -= m_cell_size;
		d_Value_Y = -1.f / m_block_size;
		// move uvs
	}
	if (d_Z >= m_cell_size) {
		m_transform.m_pos.z += m_cell_size;
		d_Value_X = 1.f / m_block_size;
		// move uvs
	}
	else if (-d_Z >= m_cell_size) {
		m_transform.m_pos.z -= m_cell_size;
		d_Value_X = -1.f / m_block_size;
		// move uvs
	}

	for (Vertex & v : m_vertices) {
		v.uv_blend.x += d_Value_X;
		v.uv_blend.y += d_Value_Y;
	}

	m_model->RebindBuffer(m_vertices);
}

void Terrain::Draw()
{
	glUseProgram(m_shader->GetProgramID());

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->GetIBO(m_is_wired));
	
	Shaders objShader = m_shader->GetShaderData();
	
	if (objShader.uvBlendAttribute != -1) {
		glEnableVertexAttribArray(objShader.uvBlendAttribute);
		glVertexAttribPointer(objShader.uvBlendAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3) * 3 + sizeof(Vector2)));
	}
	
	if (objShader.texHeightsUniform != -1) {
		glUniform3fv(objShader.texHeightsUniform, 1, &m_heights.x);
	}
	
	SharedDrawElements();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Terrain::FixedUpdate()
{
	
}

void Terrain::Destroy()
{
}

std::string Terrain::ToString()
{
	return std::string("TODO terrain string");
}

std::string Terrain::GetClassName()
{
	return std::string("Terrain");
}
