
#include "Trajectory.h"
#include <cmath>

#include "DeltaTime.h"
#include "Math.h"



Trajectory::Trajectory(int iter_count, float speed, Direction dir, Type type)
	: m_next_point(1), m_movement_dir(1), m_speed(speed), m_direction(dir), m_type(type), m_infinite(false)
{
	if (-1 == iter_count) {
		m_iterations = 1;
		m_infinite = true;
	}

}

Trajectory::~Trajectory()
{
}

void Trajectory::NextPosition(Vector3 & pos, Vector3 & rot)
{
	if (0 == m_iterations)
	{
		return;
	}

	if (m_next_point >= m_points.size() || m_next_point < 0)
	{
		if (DIR_ALTERNATE == m_direction)
		{
			m_movement_dir *= -1;
			m_next_point += m_movement_dir * 2;

			if (false == m_infinite)
			{
				--m_iterations;
			}

			if (0 == m_iterations)
			{
				return;
			}
		}
		else
		{
			if (LINE_LOOP == m_type || CIRCULAR == m_type)
			{
				m_next_point = 0;
				m_crtLerp = Vec3TimeLerp(m_points.back(), m_points[0]);
				m_lastCheckpoint = m_points.back();
			}
			else
			{
				return;
			}
		}
	}

	if (CIRCULAR != m_type)
	{
		Vector3 forward = (m_points[m_next_point] - pos).Normalize();
		Vector3 mov = forward * m_speed / DeltaTime::PHYSICS_TIME_STEP;

// 		m_crtCoef += m_speed / DeltaTime::PHYSICS_TIME_STEP;
// 
// 		Vector3 mov = Math::Lerp(pos, m_points[m_next_point], m_crtCoef);
 		pos = m_crtLerp.Step(m_speed * DeltaTime::PHYSICS_TIME_STEP);

		/// TODO: cum fac rotatia?
		//Vector3 crtLookAt = rot.Normalize();
		//float angle = acos(forward.Dot(crtLookAt));
		//Vector3 axis = forward.Cross(crtLookAt);
		//Matrix().SetRotationAngleAxis(angle, axis.x, axis.y, axis.z);
	}
	else {
		/// TODO: miscare circulara. cba
	}

	const float eps = 0.001;
	/// TODO: care ar fi un prag decent?
	if ((m_points[m_next_point] - pos).Length() < eps) {
		m_lastCheckpoint = m_points[m_next_point];
		m_next_point += m_movement_dir;
		m_crtLerp = Vec3TimeLerp(m_lastCheckpoint, m_next_point);
	}
}

void Trajectory::AddPoint(const Vector3 point)
{
	m_points.push_back(point);
}

Trajectory::Type Trajectory::GetTrajectoryTypeFromString(const std::string & type)
{
	if ("linear" == type) {
		return Type::LINEAR;
	}
	else if ("line_strip" == type) {
		return Type::LINE_STRIP;
	}
	else if ("line_loop" == type) {
		return Type::LINE_LOOP;
	}
	else if ("circle" == type) {
		return Type::CIRCULAR;
	}

	return Type::LINEAR;
}

Trajectory::Direction Trajectory::GetDirectionTypeFromString(const std::string & dir)
{
	if ("normal" == dir) {
		return Direction::DIR_NORMAL;
	}
	else if ("alternate" == dir) {
		return Direction::DIR_ALTERNATE;
	}

	return Direction::DIR_NONE;
}
