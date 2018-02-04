#include "ButtonList.h"

ButtonList::ButtonList(float top, float left, float w, float h, Design d)
	: UiElement(top, left, w, h, d)
{
}

ButtonList::~ButtonList()
{
	for (Button *btn : m_buttons)
	{
		if (nullptr != btn)
		{
			delete btn;
			btn = nullptr;
		}
	}
}

void ButtonList::Init()
{
}

void ButtonList::FixedUpdate()
{
}

void ButtonList::Update()
{
}

void ButtonList::Draw()
{
	for (Button *btn : m_buttons)
	{
		btn->Draw();
	}
}

void ButtonList::Destroy()
{
	for (Button *btn : m_buttons)
	{
		if (nullptr != btn)
		{
			delete btn;
			btn = nullptr;
		}
	}
}

std::string ButtonList::ToString()
{
	return "TODO: buttonlist tostring";
}

std::string ButtonList::GetClassName()
{
	return "ButtonList";
}

void ButtonList::AddButton(const std::string & text)
{
	float top_offset = m_buttons.size() * 260.f;
	Button *b = new Button(m_top_offset - top_offset - 250.f, m_left_offset, 2200.f, 250.f, text, { "8", Vector4(0.f, 0.f, 1.f, 1.f) });
	b->Init();

	m_buttons.push_back(b);
}
