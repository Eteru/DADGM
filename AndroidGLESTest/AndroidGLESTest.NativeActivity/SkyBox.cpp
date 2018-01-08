
#include "SkyBox.h"
#include "SceneManager.h"


SkyBox::SkyBox(Vector3 pos, Vector3 rot, Vector3 scale, std::string id,
	float offset, float size)
	: SceneObject(pos, rot, scale, false, id), m_offsetY(offset), m_size(size), m_half_size(size * 0.5f)
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::Init()
{
	m_scale = Vector3(m_size, m_size, m_size);
	// Center skybox to camera position
	Camera *cam = SceneManager::GetInstance()->GetActiveCamera();
	Vector3 camera_pos = cam->GetPosition();
	m_position.x = camera_pos.x - m_half_size;
	m_position.y = camera_pos.y - m_half_size + m_offsetY;
	m_position.z = camera_pos.z - m_half_size;
}

void SkyBox::Update()
{
	Camera *cam = SceneManager::GetInstance()->GetActiveCamera();
	Vector3 camera_pos = cam->GetPosition();
	m_position.x = camera_pos.x - m_half_size;
	m_position.y = camera_pos.y - m_half_size + m_offsetY;
	m_position.z = camera_pos.z - m_half_size;

	GeneralUpdate();
}

void SkyBox::Draw()
{
	glUseProgram(m_shader->GetProgramID());

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->GetIBO(m_is_wired));

	Shaders objShader = m_shader->GetShaderData();

	SharedDrawElements();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool SkyBox::Collides(SceneObject * obj)
{
	return false;
}

bool SkyBox::Contains(const Vector3 & point)
{
	return false;
}
