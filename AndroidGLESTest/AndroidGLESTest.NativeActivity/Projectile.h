#pragma once
#include "GameLoopObject.h"
#include "Stats.h"
#include "PhysicsBody.h"


class Projectile : public GameLoopObject, public StatHolderObject
{
public:
	Projectile();
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	size_t m_team;
	size_t m_enemyTeam;
	bool m_isSeeker;
	GameLoopObject *m_target;
	
	PhysicsBody *m_physicsBody;
};
