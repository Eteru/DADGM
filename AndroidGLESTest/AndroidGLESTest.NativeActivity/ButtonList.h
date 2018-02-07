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

	void AddButton(const std::string & text, ButtonList *bl, bool is_back = false);
	void SetClickedButton(int index);
	void SetActive(bool active) { m_active = active; }
	void SetCustomFunction(size_t index, std::function<void(int)> f);
	bool IsActive() { return m_active; }

	void SetButtonActive(int idx);

private:
	bool m_active;
	int m_current_page;
	std::vector<Button *> m_buttons;
	std::vector<ButtonList *> m_button_lists;
};