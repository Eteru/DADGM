
#include "AnimatedObject.h"

#include <ctime>
#include <cmath>

AnimatedObject::AnimatedObject(Vector3 pos, Vector3 rot, Vector3 scale, bool depth_test, std::string name, float displacement)
	: SceneObject(pos, rot, scale, name, depth_test), m_displacement_max(displacement), m_time(0.f)
{
}

AnimatedObject::~AnimatedObject()
{
}

void AnimatedObject::Init()
{
}

void AnimatedObject::Update()
{
	double intpart;
	clock_t t = clock();
	m_time = static_cast<float>(t) / CLOCKS_PER_SEC;
	m_time = std::modf(m_time, &intpart);

	GeneralUpdate();
}

void AnimatedObject::Draw()
{
	glUseProgram(m_shader->GetProgramID());

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->GetIBO(m_is_wired));

	Shaders objShader = m_shader->GetShaderData();

	if (objShader.timeUniform != -1) {
		glUniform1f(objShader.timeUniform, m_time);
	}
	if (objShader.displacementMaxUniform != -1) {
		glUniform1f(objShader.displacementMaxUniform, m_displacement_max);
	}

	SharedDrawElements();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void AnimatedObject::FixedUpdate()
{
}

void AnimatedObject::Destroy()
{
}

std::string AnimatedObject::ToString()
{
	return std::string("TODO animated object string");
}

std::string AnimatedObject::GetClassName()
{
	return std::string("AnimatedObject");
}
