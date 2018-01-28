#pragma once
#include "GameLoopObject.h"
#include "Stats.h"
#include "PhysicsBody.h"


class Projectile : public GameLoopObject
{
public:
	Projectile();
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	size_t m_team;
	size_t m_enemyTeam;
	float m_timeToLive;
	size_t m_bounces;
	bool m_isSeeker;
	float m_speed;
	GameLoopObject *m_target;

	float m_damage;

	PhysicsBody *m_physicsBody;
};
