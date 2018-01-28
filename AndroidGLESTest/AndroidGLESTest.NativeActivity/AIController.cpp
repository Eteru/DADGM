#include "AIController.h"

#include <algorithm>

#include "MovementController.h"
#include "MapManager.h"
#include "UniqueID.h"
#include "GameConstants.h"
#include "PhysicsBody.h"
#include "AIRule.h"


std::string AIController::ToString()
{
	return std::string("TODO AIController string");
}

std::string AIController::GetClassName()
{
	return std::string("AIController");
}

void AIController::Init()
{
	m_movementController = new MovementController(m_physicsBody, m_mapManager);
	m_movementController->SetID(UniqueID::GetID(m_movementController->GetClassName()));
	m_movementController->Init();

	AddComponent(m_movementController);


	MovementAIRule *defaultMovement = new MovementAIRule();
	defaultMovement->m_action = new Explore(m_movementController, m_mapManager);
	defaultMovement->m_action->SetID(UniqueID::GetID(defaultMovement->m_action->GetClassName()));
	defaultMovement->m_action->Init();
	defaultMovement->m_action->m_priority = 0;


	m_movementRules.push_back(defaultMovement);

// 	m_defaultMovementAction = new Explore(m_movementController, m_mapManager);
// 	m_defaultMovementAction->SetID(UniqueID::GetID(m_defaultMovementAction->GetClassName()));
// 	m_defaultMovementAction->Init();


// 	m_defaultMovementAction = new KeepDistance(Vector2(10, 10), 2, m_movementController, m_mapManager);
// 	m_defaultMovementAction->SetID(UniqueID::GetID(m_defaultMovementAction->GetClassName()));
// 	m_defaultMovementAction->Init();
// 	m_defaultMovementAction->m_debugDraw = true;

}

void AIController::FixedUpdate()
{
	std::sort(m_movementRules.begin(), m_movementRules.end(), AIRule::CustomCmp());

	for (auto rule : m_movementRules)
	{
		if (rule->Evaluate() && rule->m_action != m_currentMovementAction)
		{			
			if (nullptr != m_currentMovementAction)
			{
				RemoveComponent(m_currentMovementAction);
			}
			AddComponent(rule->m_action);
			m_currentMovementAction = rule->m_action;
			break;
		}
	}


	std::sort(m_itemRules.begin(), m_itemRules.end(), AIRule::CustomCmp());
	for (auto rule : m_itemRules)
	{
		/// TODO aside from evaluating, always check that the item is off-cooldown
		if (rule->Evaluate()) 
		{
			rule->m_action->Execute();
		}
	}
}
