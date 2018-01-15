#pragma once
#include <vector>

#include "GameLoopObject.h"

namespace InputManager
{
	void RegisterListener(GameLoopObject *obj);

	void TouchDown(const int x, const int y);
	void TouchUp(const int x, const int y);
	void TouchDrag(const int xPrev, const int yPrev, const int x, const int y);

	static std::vector<GameLoopObject *> m_listeners;
}