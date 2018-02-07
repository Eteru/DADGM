#include "InputManager.h"
#include "PrintUtils.h"

void InputManager::RegisterListener(GameLoopObject *obj)
{

	PrintUtils::PrintI("[INPUT] Registered object " + obj->ToString());
	m_listeners.push_back(obj);
}

void InputManager::TouchDown(const int x, const int y)
{
	//LOGI("[INPUT] Touch down at (%d, %d)", x, y);

	for (auto obj : m_listeners)
	{
		if (obj->OnTouchDown(x, y))
		{
			return;
		}
	}
}

void InputManager::TouchUp(const int x, const int y)
{
	//LOGI("[INPUT] Touch up at (%d, %d)", x, y);

	for (auto obj : m_listeners)
	{
		obj->OnTouchUp(x, y);
	}
}

void InputManager::TouchDrag(const int xPrev, const int yPrev, const int x, const int y)
{
	//LOGI("[INPUT] Touch drag from (%d, %d) to (%d, %d)", xPrev, yPrev, x, y);

	for (auto obj : m_listeners)
	{
		obj->OnTouchDrag(xPrev, yPrev, x, y);
	}
}

