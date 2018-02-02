#include "AIController.h"

#include <algorithm>

#include "MovementController.h"
#include "MapManager.h"
#include "UniqueID.h"
#include "GameConstants.h"
#include "PhysicsBody.h"
#include "AIRule.h"
#include "Robot.h"


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


// 	MovementAIRule *defaultMovement = new MovementAIRule();
// 	defaultMovement->m_action = new Explore(m_movementController, m_mapManager);
// 	defaultMovement->m_action->SetID(UniqueID::GetID(defaultMovement->m_action->GetClassName()));
// 	defaultMovement->m_action->Init();
// 
// 
// 	m_movementRules.push_back(defaultMovement);

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
		if (Evaluate(rule) && rule.m_action != m_currentMovementAction)
		{			
// 			if (nullptr != m_currentMovementAction)
// 			{
// 				RemoveComponent(m_currentMovementAction);
// 			}
// 			AddComponent(rule->m_action);
			m_currentMovementAction = rule.m_action;
			break;
		}
	}

	AIMovementActions::SetDestination(m_currentMovementAction, m_movementController, m_robot->m_target, m_mapManager);


	std::sort(m_itemRules.begin(), m_itemRules.end(), AIRule::CustomCmp());
	for (auto rule : m_itemRules)
	{
		/// TODO aside from evaluating, always check that the item is off-cooldown
		if (Evaluate(rule)) 
		{
			rule.m_item->Activate();
		}
	}
}

bool AIController::Evaluate(const AIRule rule) const
{
	bool result = true;

	for (Term term : rule.m_terms)
	{
		result &= Evaluate(term);
	}

	return result;
}

bool AIController::Evaluate(const Term term) const
{
	float lhs;

	switch (term.m_lhsType)
	{ 
	case LHSType::DISTANCE_FROM_TARGET:
		lhs = m_robot->MapDistanceToTarget();
		break;
	case LHSType::STAT_VALUE:
		lhs = m_robot->m_stats.at(xmlTypeMap.at(term.m_lhsName)).GetValue();
		break;
	case LHSType::ITEM_READY:
		return m_robot->FindActive(term.m_lhsName)->m_isAvailable;
	default:
		break;
	}

	switch (term.m_opType)
	{
	case OPType::LT:
		return lhs < term.m_rhs;
	case OPType::GT:
		return lhs > term.m_rhs;
	case OPType::EQ:
		return lhs == term.m_rhs;
	case OPType::LEQ:
		return lhs <= term.m_rhs;
	case OPType::GEQ:
		return lhs >= term.m_rhs;
	default:
		break;
	}

	return false;
}

void AIController::AcquireTarget()
{

}
