#pragma once

#include "GameLoopObject.h"
#include "AIRule.h"

class MovementController;
class MapManager;
class Robot;
class PhysicsBody;
class Buff;

class AIController : public GameLoopObject
{
public:

	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	Vector2 m_currentDest;
	bool m_hasDest;
	MovementController *m_movementController;
	MapManager *m_mapManager;
	Robot *m_robot;
	PhysicsBody *m_physicsBody;
	


	virtual void Init() override;


	virtual void FixedUpdate() override;


	bool Evaluate(const AIRule rule) const;
	bool Evaluate(const Term term) const;

	void AcquireTarget();

	std::vector<Buff> m_buffs;

	MovementActionType m_currentMovementAction;	

	std::vector<ItemAIRule> m_itemRules;
	std::vector<MovementAIRule> m_movementRules;
};