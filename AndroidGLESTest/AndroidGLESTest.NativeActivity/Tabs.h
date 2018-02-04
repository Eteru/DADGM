#pragma once
#include "UiElement.h"
#include "ButtonList.h"

#include <vector>

class Tabs :
	public UiElement
{
public:
	Tabs(float top, float left, float w, float h, Design d);
	virtual ~Tabs();

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

	void AddTab(const std::string & name, ButtonList *bl);
	void SetActiveTab(size_t index);

private:
	size_t m_active_tab;
	std::vector<Button *> m_tabs;
	std::vector<ButtonList *> m_tabs_content;


};

