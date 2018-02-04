#include "PhysicsBody.h"
#include "DebugDrawPrimitives.h"



PhysicsBody::PhysicsBody()
{
	m_kinematic = false;
	m_damping = 0.1f;
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

		float timeStepAngular = m_turningAcceleration * GameConstants::PHYSICS_TIME_STEP * 2 * Math::PI;
		Vector3 rotationStep = Math::GetAlignmentRotation(m_transform.GetForward(), dir);
		

		if (rotationStep.Length() <= Math::Length(m_angularVelEngine))
		{
			m_angularVelEngine = -m_transform.GetWorldRot() + Math::GetAlignmentRotation(GameConstants::DEFAULT_FORWARD, dir);
		}
		else
		{
			rotationStep = Math::ClampLength(rotationStep, timeStepAngular);
			m_angularVelEngine = Math::ClampLength(m_angularVelEngine + rotationStep, m_topTurningSpeed * GameConstants::PHYSICS_TIME_STEP * 2 * Math::PI);
		}

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

void PhysicsBody::ApplyLinearImpulse(const Vector3 impulse)
{
	m_linearVelImpact += impulse / m_mass;
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

PhysicsBodyDumb::PhysicsBodyDumb()
{		
}

void PhysicsBodyDumb::FixedUpdate()
{
	m_linearVelImpact = Math::SetLength(m_linearVelImpact, m_topSpeed * GameConstants::PHYSICS_TIME_STEP);


// 	Vector3 axis = Math::Cross(m_linearVelImpact, GameConstants::DEFAULT_FORWARD);
// 
// 	if (Math::Length(axis) != 0)
// 	{
// 		m_transform.SetRot(std::acos(Math::Dot(GameConstants::DEFAULT_FORWARD, Math::Normalize(m_linearVelImpact))) * Math::Normalize(axis));
// 	}

	///TODO proper rotation so that forward is always in the movement direction
	m_transform.SetRot(Math::GetAlignmentRotation(GameConstants::DEFAULT_FORWARD, m_linearVelImpact));

	m_transform.SetPos(m_transform.GetWorldPos() + m_linearVelImpact);
}

std::string PhysicsBodyDumb::ToString()
{
	return std::string("TODO PhysicsBodyDumb string");
}

std::string PhysicsBodyDumb::GetClassName()
{
	return std::string("PhysicsBodyDumb");
}

void PhysicsBodyDumb::DebugDraw()
{
	
}

void PhysicsBodyDumb::Init()
{
	Vector3 dir = Math::Normalize(m_initialTarget - m_transform.GetWorldPos());


	m_angularVelImpact = Vector3(0, std::acos(Math::Dot(m_transform.GetForward(), dir)), 0);
	m_linearVelImpact = Math::SetLength(dir, m_topSpeed * GameConstants::PHYSICS_TIME_STEP);
}
