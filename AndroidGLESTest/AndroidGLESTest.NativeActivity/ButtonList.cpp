#include "ButtonList.h"

ButtonList::ButtonList(float top, float left, float w, float h, Design d)
	: UiElement(top, left, w, h, d), m_current_page(-1), m_active(false)
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
	if (true == m_active)
	{
		for (Button *btn : m_buttons)
		{
			btn->Draw();
		}
	}
	else
	{
		m_button_lists[m_current_page]->Draw();
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

void ButtonList::AddButton(const std::string & text, ButtonList *bl, bool is_back)
{
	float top_offset = m_buttons.size() * 260.f;
	Button *b = new Button(m_top_offset - top_offset - 250.f, m_left_offset, 2200.f, 250.f, text, { "8", Vector4(0.f, 0.f, 1.f, 1.f) });
	b->SetParent(this);
	b->SetIndex(is_back == true ? -1 : m_buttons.size());
	b->SetCallbackFunction(std::bind(&ButtonList::SetClickedButton, this, std::placeholders::_1));
	b->Init();
	m_buttons.push_back(b);

	if (nullptr != bl)
	{
		bl->SetParent(this);
		bl->AddButton("Back", nullptr, true);
	}

	m_button_lists.push_back(bl);
}

void ButtonList::SetClickedButton(int index)
{
	if (true == m_active)
	{
		m_current_page = index;

		if (index < 0 && nullptr != m_parent)
		{
			SetActive(false);
			dynamic_cast<ButtonList *>(m_parent)->SetActive(true);

			return;
		}

		if (nullptr != m_button_lists[index])
		{
			SetActive(false);
			m_button_lists[index]->SetActive(true);
		}
	}
}
