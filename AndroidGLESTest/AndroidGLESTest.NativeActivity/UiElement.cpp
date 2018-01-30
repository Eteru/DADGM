
#include "UiElement.h"

UiElement::UiElement()
	: GameLoopObject(), m_top_offset(0.f), m_left_offset(0.f), m_width(0.f), m_height(0.f)
{
}

UiElement::UiElement(float top, float left, float w, float h)
	: GameLoopObject(), m_top_offset(top), m_left_offset(left), m_width(w), m_height(h)
{
}

UiElement::~UiElement()
{
}

void UiElement::Init()
{
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
