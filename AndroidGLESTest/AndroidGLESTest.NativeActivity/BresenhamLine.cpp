#include "BresenhamLine.h"

std::vector<Vector2> BresenhamLine::LineLow(Vector2 from, Vector2 to)
{
	std::vector<Vector2> result;
	int dx = to.x - from.x;
	int dy = to.y - from.y;

	int	yi = 1;

	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}

	int D = 2 * dy - dx;
	int y = from.y;


	for (int x = from.x; x <= to.x; ++x)
	{
		result.push_back(Vector2(x, y));

		if (D > 0)
		{
			y += yi;
			D -= 2 * dx;
		}

		D += 2 * dy;
	}

	return result;
}

std::vector<Vector2> BresenhamLine::LineHigh(Vector2 from, Vector2 to)
{
	std::vector<Vector2> result;
	int dx = to.x - from.x;
	int dy = to.y - from.y;

	int	xi = 1;

	if (dx < 0)
	{
		xi = -1;
		dx = -dx;
	}

	int D = 2 * dx - dy;
	int x = from.x;


	for (int y = from.y; y <= to.y; ++y)
	{
		result.push_back(Vector2(x, y));

		if (D > 0)
		{
			x += xi;
			D -= 2 * dy;
		}

		D += 2 * dx;
	}

	return result;
}

std::vector<Vector2> BresenhamLine::Line(Vector2 from, Vector2 to)
{
	if (std::abs(to.y - from.y) < std::abs(to.x - from.x))
	{
		if (from.x > to.x)
			return LineLow(to, from);

		return LineLow(from, to);
	}
	else
	{
		if (from.y > to.y)
			return LineHigh(to, from);

		return LineHigh(from, to);
	}
}
