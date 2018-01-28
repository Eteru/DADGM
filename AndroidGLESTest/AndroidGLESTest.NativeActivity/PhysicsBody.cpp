#include "PhysicsBody.h"
#include "DebugDrawPrimitives.h"



PhysicsBody::PhysicsBody()
{
	m_kinematic = false;
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

	//m_debugDraw = true;
}

void PhysicsBody::FixedUpdate()
{
	if (m_kinematic)
	{
		return;
	}


	Vector3 dir = m_targetPos - m_transform.GetWorldPos();



	if (m_hasTarget && dir.Length() > 0)
	{
		Vector3 fw = m_transform.GetForward();

		float dt = Math::Clamp(Math::Dot(fw, Math::Normalize(dir)), -1.f, 1.f);

		float rads = std::acos(dt);

		Vector3 crss = Math::Normalize(Math::Cross(fw, Math::Normalize(dir)));
		if (Math::Dot(crss, m_transform.GetUp()) == -1)
		{
			rads = -rads;
		}

		float timeStepAngular = m_turningAcceleration * GameConstants::PHYSICS_TIME_STEP * rads;

		if (std::abs(rads) < 0.05)
		{
			Vector3 rotatedFw = Math::RotateAround(fw, m_transform.GetUp(), rads);
			m_angularVelEngine.y = std::acos(Math::Clamp(Math::Dot(fw, rotatedFw), -1.f, 1.f)) * Math::Sign(timeStepAngular);
		}
		else
		{
			Vector3 rotatedFw = Math::RotateAround(fw, m_transform.GetUp(), timeStepAngular);
			m_angularVelEngine.y += std::acos(Math::Clamp(Math::Dot(fw, rotatedFw), -1.f, 1.f)) * Math::Sign(timeStepAngular);
		}

		m_angularVelEngine = Math::ClampLength(m_angularVelEngine, m_topTurningSpeed * GameConstants::PHYSICS_TIME_STEP);

		Vector3 timeStepSpeed = Math::SetLength(dir, m_acceleration * GameConstants::PHYSICS_TIME_STEP);
		//m_linearVelEngine *= (1.f - m_damping) * (1.f - std::abs(dt));
		m_linearVelEngine += timeStepSpeed * dt;
		m_linearVelEngine = Math::ClampLength(m_linearVelEngine, m_topSpeed * GameConstants::PHYSICS_TIME_STEP);
	}
	else
	{
		m_linearVelEngine *= (1.f - m_damping);
		if (m_linearVelEngine.Length() <= 0.003f)
		{
			m_linearVelEngine = Vector3(0.f);
		}

		m_angularVelEngine *= (1.f - m_damping);
		if (m_angularVelEngine.Length() <= 0.003f)
		{
			m_angularVelEngine = Vector3(0.f);
		}
	}


	Vector3 ps = m_transform.GetWorldPos() + m_linearVelEngine + m_linearVelImpact;
	ps.y = GameConstants::WALL_HEIGHT;

	Vector3 rot = m_transform.GetWorldRot() + m_angularVelEngine + m_angularVelImpact;

	m_transform.SetPos(ps);
	m_transform.SetRot(rot);

	m_angularVelImpact *= (1.f - m_damping);
	m_linearVelImpact *= (1.f - m_damping);

	if (Math::Length(m_targetPos - m_transform.GetWorldPos()) < GameConstants::CELL_SIZE / 2)
	{
		m_hasTarget = false;
	}

	//m_transform.SetRot(m_transform.GetWorldRot() + Vector3(0.f, 0.04f, 0.f));
}

std::string PhysicsBody::ToString()
{
	return std::string("TODO PhysicsBody string");
}

std::string PhysicsBody::GetClassName()
{
	return std::string("PhysicsBody");
}

void PhysicsBody::OnCollision(PhysicsBody *other)
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
