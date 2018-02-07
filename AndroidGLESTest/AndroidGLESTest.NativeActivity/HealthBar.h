#pragma once
#include "UiElement.h"
#include "Robot.h"

class HealthBar
	: public UiElement
{
public:
	HealthBar(float top, float left, float w, float h, std::string text, UiElement::Design d);
	virtual ~HealthBar();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() override;

private:
	float m_init_health;
	std::string m_text;
	Matrix m_M;
	Robot *m_robot;
};

