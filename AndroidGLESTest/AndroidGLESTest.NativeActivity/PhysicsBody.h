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


	void ApplyLinearImpulse(const Vector3 impulse);
	void OnCollision(PhysicsBody *other);
	void SetTarget(Vector3 worldPos);

	Vector3 m_targetPos;

	GameLoopObject *m_linkedObject;

	bool m_hasTarget;

	bool m_kinematic;

	Vector3 m_linearVelEngine;
	Vector3 m_linearVelImpact;

	Vector3 m_angularVelEngine;
	Vector3 m_angularVelImpact;

	Vector3 m_forward;
	float m_acceleration;
	float m_topSpeed;
	float m_turningAcceleration;
	float m_topTurningSpeed;
	float m_mass;
	float m_damping;
	float m_inertia;

	virtual void Draw() override;


	virtual void DebugDraw() override;

};

class PhysicsBodyDumb : public PhysicsBody
{

public:

	PhysicsBodyDumb();
	virtual void FixedUpdate() override;
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;


	virtual void DebugDraw() override;

	Vector3 m_initialTarget;


	virtual void Init() override;

};