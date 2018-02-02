#pragma once
#include "UiElement.h"
#include "ResourceManager.h"

class Button :
	public UiElement
{
public:
	Button();
	Button(float top, float left, float w, float h, std::string text, UiElement::Design d);
	virtual ~Button();

	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void DebugDraw() override;
	virtual void Destroy() override;

	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	virtual void OnTouchDown(const int x, const int y) override;
	virtual void OnTouchUp(const int x, const int y) override;
	virtual void OnTouchDrag(const int xPrev, const int yPrev, const int x, const int y) override;

private:
	std::string m_text;
};

