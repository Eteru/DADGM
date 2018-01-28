#pragma once
#include "AIActions.h"

class Term
{
public:
	virtual bool Evaluate() { return true; }
};

class LessThanTerm : public Term
{
public:
	virtual bool Evaluate() override;

	float m_lhs;
	float m_rhs;
};

class GreaterThanTerm : public Term
{
public:
	virtual bool Evaluate() override;

	float m_lhs;
	float m_rhs;
};

class EqualsTerm: public Term
{
public:
	virtual bool Evaluate() override;

	float m_lhs;
	float m_rhs;
};

class AIRule
{
public:
	~AIRule();
	bool Evaluate();
	std::vector<Term> m_terms;
	AIAction *m_action;

	typedef struct 
	{
		bool operator()(const AIRule *a, const AIRule *b)
		{
			return a->m_action->m_priority > b->m_action->m_priority;
		}

		bool operator()(const AIRule a, const AIRule b)
		{
			return a.m_action->m_priority > b.m_action->m_priority;
		}
	} CustomCmp;
};

class MovementAIRule : public AIRule
{
public:
	MovementAction *GetAction();

	MovementAction * m_action;
};

class ItemAIRule : public AIRule
{
public:
	void Execute();

	ItemAction *m_action;
};
