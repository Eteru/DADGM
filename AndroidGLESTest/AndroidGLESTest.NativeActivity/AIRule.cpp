#include "AIRule.h"

bool LessThanTerm::Evaluate()
{
	return m_lhs < m_rhs;
}

bool GreaterThanTerm::Evaluate()
{
	return m_lhs > m_rhs;
}

bool EqualsTerm::Evaluate()
{
	return m_lhs == m_rhs;
}

AIRule::~AIRule()
{
	if (m_action != nullptr)
	{
		delete m_action;
		m_action = nullptr;
	}
}

bool AIRule::Evaluate()
{
	bool result = true;

	for (auto term : m_terms)
	{
		result &= term.Evaluate();
	}

	return result;
}

void ItemAIRule::Execute()
{
	m_action->Execute();
}

MovementAction * MovementAIRule::GetAction()
{
	return m_action;
}
