
#include "Tabs.h"
#include <functional>

Tabs::Tabs(float top, float left, float w, float h, Design d)
	: UiElement(top, left, w, h, d), m_active_tab(0)
{
}


Tabs::~Tabs()
{
	for (Button *btn : m_tabs)
	{
		if (nullptr != btn)
		{
			delete btn;
			btn = nullptr;
		}
	}

	for (ButtonList *bl : m_tabs_content)
	{
		if (nullptr != bl)
		{
			delete bl;
			bl = nullptr;
		}
	}
}

void Tabs::Init()
{
	//ConvertToScreenValues();
}

void Tabs::FixedUpdate()
{
}

void Tabs::Update()
{
}

void Tabs::Draw()
{
	for (Button *btn : m_tabs)
	{
		btn->Draw();
	}

	if (nullptr != m_tabs_content[m_active_tab])
	{
		m_tabs_content[m_active_tab]->Draw();
	}
}

void Tabs::Destroy()
{
	for (Button *btn : m_tabs)
	{
		if (nullptr != btn)
		{
			delete btn;
			btn = nullptr;
		}
	}

	for (ButtonList *bl : m_tabs_content)
	{
		if (nullptr != bl)
		{
			delete bl;
			bl = nullptr;
		}
	}
}

std::string Tabs::ToString()
{
	return "TODO tabs tostring";
}

std::string Tabs::GetClassName()
{
	return "Tabs";
}

void Tabs::AddTab(const std::string & name, ButtonList *bl)
{
	float left_offset = m_tabs.size() * 750.f;
	Button *b = new Button(m_top_offset - 250.f, m_left_offset + left_offset, 700.f, 250.f, name, { "8", Vector4(0.f, 0.f, 1.f, 1.f) });
	b->SetIndex(m_tabs.size());
	b->SetCallbackFunction(std::bind(&Tabs::SetActiveTab, this, std::placeholders::_1));
	b->Init();

	m_tabs.push_back(b);
	m_tabs_content.push_back(bl);
}

void Tabs::SetActiveTab(int index)
{
	m_tabs_content[m_active_tab]->SetActive(false);
	m_tabs_content[index]->SetActive(true);

	m_active_tab = index;
}
