#include "PhysicsBody.h"
#include "DebugDrawPrimitives.h"



PhysicsBody::PhysicsBody()
{
	m_kinematic = true;
	m_damping = 0.02f;
	m_targetPos = Vector3(0.f);
	m_linearVelEngine = Vector3(0.f);
	m_linearVelImpact = Vector3(0.f);
	m_angularVelEngine = Vector3(0.f);
	m_angularVelImpact = Vector3(0.f);
	m_hasTarget = false;
}

void PhysicsBody::Init()
{

	m_debugDraw = true;
}

void PhysicsBody::FixedUpdate()
{
	if (!m_kinematic)
	{
		return;
	}


	Vector3 dir = m_targetPos - m_transform.GetWorldPos();



	if (m_hasTarget && dir.Length() > 0)
	{
		Vector3 timeStepSpeed = Math::SetLength(dir, m_acceleration * GameConstants::PHYSICS_TIME_STEP);

		m_linearVelEngine += timeStepSpeed;
		m_linearVelEngine = Math::ClampLength(m_linearVelEngine, m_topSpeed * GameConstants::PHYSICS_TIME_STEP);
	}
	else
	{
		m_linearVelEngine -= m_linearVelEngine * m_damping;
		if (m_linearVelEngine.Length() <= 0.003f)
		{
			m_linearVelEngine = Vector3(0.f);
		}
	}

	m_linearVelImpact -= m_linearVelImpact * m_damping;

	Vector3 ps = m_transform.GetLocalPos() + m_linearVelEngine + m_linearVelImpact;
	ps.y = GameConstants::WALL_HEIGHT;

	m_transform.SetPos(ps);

	if (Math::Length(m_targetPos - m_transform.GetWorldPos()) < GameConstants::CELL_SIZE / 2)
	{
		m_hasTarget = false;
	}
}

std::string PhysicsBody::ToString()
{
	return std::string("TODO PhysicsBody string");
}

std::string PhysicsBody::GetClassName()
{
	return std::string("PhysicsBody");
}

void PhysicsBody::OnContact(PhysicsBody *other)
{
	///TODO notify m_linkedObject of contact
}

void PhysicsBody::SetTarget(Vector3 worldPos)
{
	m_targetPos = worldPos;
	m_hasTarget = true;
}

void PhysicsBody::Draw()
{

}

void PhysicsBody::DebugDraw()
{
	if (m_hasTarget)
	{
		DebugDrawPrimitives::DrawLine(m_transform.GetWorldPos(), m_targetPos, DebugDrawPrimitives::COLOR_RED);
	}
}
