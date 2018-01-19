#include "BoundingSphere.h"

BoundingSphere::BoundingSphere() : m_radius(1.f)
{

}

BoundingSphere::BoundingSphere(GLfloat radius) : m_radius(radius)
{

}

std::string BoundingSphere::ToString()
{
	return std::string("TODO BoundingSphere String");
}

std::string BoundingSphere::GetClassName()
{
	return std::string("BoundingSphere");
}
