#pragma once
#include "UiElement.h"
#include "ResourceManager.h"

#include <functional>

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

	virtual bool OnTouchDown(const int x, const int y) override;
	virtual void OnTouchUp(const int x, const int y) override;
	virtual void OnTouchDrag(const int xPrev, const int yPrev, const int x, const int y) override;

	inline void SetIndex(int idx) { m_index = idx; }
	inline void SetActive(bool active) { m_active = active;  }
	inline void SetCallbackFunction(std::function<void(int)> f) { m_callback_func = f; }

private:
	bool m_active;
	int m_index; /* if needed by lists or tabs */
	Matrix m_M;
	std::string m_text;
	std::vector<std::string> m_info_texts;

	std::function<void(size_t)> m_callback_func;
};

