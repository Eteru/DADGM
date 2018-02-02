#pragma once
#include <vector>

#include "Math.h"

class BresenhamLine
{
public:
	static std::vector<Vector2> LineLow(Vector2 from, Vector2 to);
	static std::vector<Vector2> LineHigh(Vector2 from, Vector2 to);
	static std::vector<Vector2> Line(Vector2 from, Vector2 to);
};