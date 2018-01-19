#pragma once
#include "GameLoopObject.h"


class PhysicsBody : public GameLoopObject
{

public:
	PhysicsBody();
	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	bool m_kinematic;

	Vector3 m_forward;

	Vector3 m_linearVel;
	Vector3 m_angularVel;
	float m_acceleration;
	float m_topSpeed;
	float m_turningSpeed;
	float m_mass;
};
