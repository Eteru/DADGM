#pragma once

#include "GameLoopObject.h"
#include "AIRule.h"

class MovementController;
class MapManager;
class Robot;
class PhysicsBody;
class MovementAction;

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


	MovementAction *m_currentMovementAction;
	ItemAction *m_currentItemAction;

	std::vector<ItemAIRule *> m_itemRules;
	std::vector<MovementAIRule *> m_movementRules;
};