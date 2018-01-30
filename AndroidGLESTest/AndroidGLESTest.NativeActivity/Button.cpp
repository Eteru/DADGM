
#include "Button.h"

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
	std::vector<Vector3> verts = {
		Vector3(m_top_offset + m_height, m_left_offset, 0.f),
		Vector3(m_top_offset, m_left_offset, 0.f),
		Vector3(m_top_offset, m_left_offset + m_width, 0.f),
		Vector3(m_top_offset + m_height, m_left_offset + m_width, 0.f)
	};

	std::vector<int> indices = {0, 1, 2, 0, 2, 3};
	m_ibo_count = indices.size();

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * verts.size(), &verts[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * m_ibo_count, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_shader = ResourceManager::GetInstance()->LoadShader("ui");
}

void Button::FixedUpdate()
{
}

void Button::Update()
{
}

void Button::Draw()
{
	// TODO: use an actual shader
	glUseProgram(m_shader->GetProgramID());
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	Shaders objShader = m_shader->GetShaderData();
	if (objShader.positionAttribute != -1)
	{
		glEnableVertexAttribArray(objShader.positionAttribute);
		glVertexAttribPointer(objShader.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
	}

	glDrawElements(
		GL_TRIANGLES,      // mode
		m_ibo_count,    // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0           // element array buffer offset
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
