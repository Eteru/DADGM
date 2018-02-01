
#include "UiElement.h"
#include "ResourceManager.h"

UiElement::UiElement()
	: GameLoopObject(), m_top_offset(0.f), m_left_offset(0.f), m_width(0.f), m_height(0.f), m_shader(nullptr)
{
}

UiElement::UiElement(float top, float left, float w, float h)
	: GameLoopObject(), m_top_offset(top), m_left_offset(left), m_width(w), m_height(h), m_shader(nullptr)
{
}

UiElement::~UiElement()
{
	glDeleteBuffers(1, &m_vbo);
}

void UiElement::Init()
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

void UiElement::SetShader(const std::string & shader_id)
{
	m_shader = ResourceManager::GetInstance()->LoadShader(shader_id);
}
