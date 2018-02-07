
#include "HealthBar.h"
#include "StringRenderer.h"

HealthBar::HealthBar(float top, float left, float w, float h, std::string text, UiElement::Design d)
	: UiElement(top, left, w, h, d), m_text(text)
{

}

HealthBar::~HealthBar()
{
}

void HealthBar::Update()
{
	if (nullptr == m_robot)
	{
		return;
	}

 	float perc = m_robot->m_stats.at(StatType::HEALTH).GetValue() / m_init_health;
	m_transform.SetScale(Vector3(perc, 1.f, 1.f));

	//m_M = Matrix().SetScale(perc, 1.f, 1.f) /* Matrix().SetTranslation(m_left_offset, m_top_offset, 0.f)*/;
}

void HealthBar::Init()
{
	ConvertToScreenValues();
	std::vector<GLfloat> verts = {
		m_left_offset, m_top_offset,
		m_left_offset, m_top_offset + m_height,
		m_left_offset + m_width, m_top_offset,
		m_left_offset + m_width, m_top_offset + m_height
	};


	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verts.size(), &verts[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	SetShader("4");
}

void HealthBar::Draw()
{
	if (nullptr == m_robot)
	{
		return;
	}

	glDisable(GL_TEXTURE_2D);
	glUseProgram(m_shader->GetProgramID());
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	Shaders objShader = m_shader->GetShaderData();
	if (objShader.positionAttribute != -1)
	{
		glEnableVertexAttribArray(objShader.positionAttribute);
		glVertexAttribPointer(objShader.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (objShader.matrixUniform != -1)
	{
		glUniformMatrix4fv(objShader.matrixUniform, 1, GL_FALSE, reinterpret_cast<GLfloat*>(m_transform.GetModel().m));
	}

	if (objShader.colorUniform != -1)
	{
		glUniform4fv(objShader.colorUniform, 1, &m_design.text_color.x);
	}

	if (objShader.hasTextureUniform != -1)
	{
		glUniform1i(objShader.hasTextureUniform, 0);
	}

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnable(GL_TEXTURE_2D);

	if ("" != m_text)
	{
		StringRenderer::DrawText(m_top_offset + 0.1f, m_left_offset, 8, m_design.text_color, m_text);
	}
}

void HealthBar::InitRobot(Robot * r)
{
	m_robot = r;
	if (nullptr != r)
	{
		m_init_health = m_robot->m_stats.at(StatType::HEALTH).GetValue();
	}
}

