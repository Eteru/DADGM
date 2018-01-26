#include "BoundingSphere.h"
#include "DebugDrawPrimitives.h"

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

void BoundingSphere::Draw()
{
	DebugDrawPrimitives::DrawSphere(m_transform.GetWorldPos(), m_transform.GetWorldRot(), m_radius * m_transform.GetWorldScale(), DebugDrawPrimitives::COLOR_BLUE);
}
