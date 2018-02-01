
#include "Button.h"
#include "StringRenderer.h"

Button::Button()
	: UiElement(), m_text("")
{
}

Button::Button(float top, float left, float w, float h, std::string text)
	: UiElement(top, left, w, h), m_text(text)
{
}

Button::~Button()
{
}

void Button::Init()
{
	std::vector<GLfloat> verts = {
		m_top_offset, m_left_offset,
		m_top_offset + m_height, m_left_offset,
		m_top_offset, m_left_offset + m_width,
		m_top_offset + m_height, m_left_offset + m_width
	};


	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verts.size(), &verts[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	SetShader("4");
}

void Button::FixedUpdate()
{
}

void Button::Update()
{
}

void Button::Draw()
{
	glUseProgram(m_shader->GetProgramID());
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	Shaders objShader = m_shader->GetShaderData();
	if (objShader.positionAttribute != -1)
	{
		glEnableVertexAttribArray(objShader.positionAttribute);
		glVertexAttribPointer(objShader.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	StringRenderer::DrawText(0, 0, 16, "0");
}

void Button::DebugDraw()
{
}

void Button::Destroy()
{
}

std::string Button::ToString()
{
	return "TODO Button ToString";
}

std::string Button::GetClassName()
{
	return "Button";
}

void Button::OnTouchDown(const int x, const int y)
{
	// todo
	// here it should do stuff
	// register a lambda function?
}

void Button::OnTouchUp(const int x, const int y)
{
}

void Button::OnTouchDrag(const int xPrev, const int yPrev, const int x, const int y)
{
}
