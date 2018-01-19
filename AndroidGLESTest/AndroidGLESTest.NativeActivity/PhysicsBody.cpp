#include "PhysicsBody.h"

PhysicsBody::PhysicsBody()
{
	m_transform.m_lerp = false;
}

void PhysicsBody::Init()
{
}

void PhysicsBody::FixedUpdate()
{
}

std::string PhysicsBody::ToString()
{
	return std::string("TODO PhysicsBody string");
}

std::string PhysicsBody::GetClassName()
{
	return std::string("PhysicsBody");
}
