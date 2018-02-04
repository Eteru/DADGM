#pragma once
#include "UiElement.h"
#include "Button.h"

#include <vector>

class ButtonList :
	public UiElement
{
public:
	ButtonList(float top, float left, float w, float h, Design d);
	virtual ~ButtonList();

	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Destroy() override;

	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	//virtual void OnTouchDown(const int x, const int y) override;
	//virtual void OnTouchUp(const int x, const int y) override;
	//virtual void OnTouchDrag(const int xPrev, const int yPrev, const int x, const int y) override;

	void AddButton(const std::string & text);

private:
	std::vector<Button *> m_buttons;
};