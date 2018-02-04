
#include "Button.h"
#include "StringRenderer.h"
#include "InputManager.h"
#include "SceneManager.h"

Button::Button()
	: UiElement(), m_text("")
{
}

Button::Button(float top, float left, float w, float h, std::string text, UiElement::Design d)
	: UiElement(top, left, w, h, d), m_text(text)
{
	InputManager::RegisterListener(this);
}

Button::~Button()
{
}

void Button::Init()
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


	std::vector<GLfloat> texcoords = {
		0.f, 1.f,
		0.f, 0.f,
		1.f, 1.f,
		1.f, 0.f
	};

	SetTexCoords(texcoords);

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

	Texture *tex = ResourceManager::GetInstance()->LoadTexture(m_design.tex_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(tex->GetTextureType(), tex->GetID());

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	Shaders objShader = m_shader->GetShaderData();
	if (objShader.positionAttribute != -1)
	{
		glEnableVertexAttribArray(objShader.positionAttribute);
		glVertexAttribPointer(objShader.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_tex_vbo);

	if (objShader.uvAttribute != -1)
	{
		glEnableVertexAttribArray(objShader.uvAttribute);
		glVertexAttribPointer(objShader.uvAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (objShader.colorUniform != -1)
	{
		Vector4 color = Vector4(0.f, 1.f, 0.f, 1.f);
		glUniform4fv(objShader.colorUniform, 1, &color.x);
	}
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if ("" != m_text)
	{
		StringRenderer::DrawText(m_top_offset + m_height * 0.35f, m_left_offset + 0.02f, 8, m_design.text_color, m_text);
	}
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
	const engine *eng = SceneManager::GetInstance()->GetEngine();
	float screenX = Math::ChangeInterval(0, eng->width, -1.f, 1.f, x);
	float screenY = -Math::ChangeInterval(0, eng->height, -1.f, 1.f, y);

	if (screenX >= m_left_offset && screenX <= (m_left_offset + m_width) &&
		screenY >= m_top_offset && screenY <= (m_top_offset + m_height))
	{

		if (m_callback_func)
		{
			m_callback_func(m_index);
		}
	}
}

void Button::OnTouchUp(const int x, const int y)
{
}

void Button::OnTouchDrag(const int xPrev, const int yPrev, const int x, const int y)
{
}
