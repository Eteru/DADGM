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

	void InitRobot(Robot *r);

private:
	float m_init_health;
	float m_health_left;
	std::string m_text;
	std::string m_output_text;
	Robot *m_robot;
};

