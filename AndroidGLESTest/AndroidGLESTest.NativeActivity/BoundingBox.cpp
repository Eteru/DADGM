
#include "BoundingBox.h"

BoundingBox::BoundingBox()
	: m_halfExtents(Vector3(0.5f))
{}

BoundingBox::BoundingBox(Vector3 halfExtents)
	: m_halfExtents(halfExtents)
{}


BoundingBox::~BoundingBox()
{
}

std::string BoundingBox::ToString()
{
	return "TODO BoundingBox string";
}

std::string BoundingBox::GetClassName()
{
	return "BoundingBox";
}
