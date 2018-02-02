
#include "UiElement.h"
#include "ResourceManager.h"
#include "SceneManager.h"

UiElement::UiElement()
	: GameLoopObject(), m_top_offset(0.f), m_left_offset(0.f), m_width(0.f), m_height(0.f), m_design({}), m_shader(nullptr)
{
}

UiElement::UiElement(float top, float left, float w, float h, Design d)
	: GameLoopObject(), m_top_offset(top), m_left_offset(left), m_width(w), m_height(h), m_design(d), m_shader(nullptr)
{
}

UiElement::~UiElement()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_tex_vbo);
}

void UiElement::Init()
{
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
}

void UiElement::FixedUpdate()
{
}

void UiElement::Update()
{
}

void UiElement::Draw()
{
}

void UiElement::DebugDraw()
{
}

void UiElement::Destroy()
{
}

std::string UiElement::ToString()
{
	return "TODO UiElement string";;
}

std::string UiElement::GetClassName()
{
	return "UiElement";
}

void UiElement::OnTouchDown(const int x, const int y)
{
}

void UiElement::OnTouchUp(const int x, const int y)
{
}

void UiElement::OnTouchDrag(const int xPrev, const int yPrev, const int x, const int y)
{
}

void UiElement::ConvertToScreenValues()
{
	const engine *eng = SceneManager::GetInstance()->GetEngine();
	if (nullptr != eng) {
		m_top_offset = Math::ChangeInterval(0, eng->height, -1.f, 1.f, m_top_offset);
		m_left_offset = Math::ChangeInterval(0, eng->width, -1.f, 1.f, m_left_offset);
		m_width = Math::ChangeInterval(0, eng->width, 0.f, 2.f, m_width);
		m_height = Math::ChangeInterval(0, eng->height, 0.f, 2.f, m_height);
	}
}

void UiElement::SetTexCoords(const std::vector<GLfloat>& coords)
{
	glGenBuffers(1, &m_tex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_tex_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * coords.size(), &coords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void UiElement::SetShader(const std::string & shader_id)
{
	m_shader = ResourceManager::GetInstance()->LoadShader(shader_id);
}
