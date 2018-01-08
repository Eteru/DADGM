#pragma once

#include <string>
#include <vector>
#include "Math.h"

class Trajectory
{
public:
	enum Direction { DIR_NORMAL = 0, DIR_ALTERNATE, DIR_NONE };
	enum Type { LINEAR = 0, LINE_STRIP, LINE_LOOP, CIRCULAR };
	Trajectory(int iter_count, float speed, Direction dir, Type type);
	virtual ~Trajectory();

	void NextPosition(Vector3 & pos, Vector3 & rot);
	void AddPoint(const Vector3 point);

	static Type GetTrajectoryTypeFromString(const std::string & type);
	static Direction GetDirectionTypeFromString(const std::string & dir);

private:
	bool m_infinite;
	int8_t m_movement_dir;
	int m_iterations;
	float m_speed;
	Direction m_direction;
	Type m_type;
	size_t m_next_point;
	std::vector<Vector3> m_points;
};

