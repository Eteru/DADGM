#pragma once
#include "AIActions.h"
#include <vector>

enum class LHSType { DISTANCE_FROM_TARGET = 0, STAT_VALUE, ITEM_READY, NUM_LHS_TYPE };
enum class OPType { LT, GT, EQ, LEQ, GEQ };

const std::unordered_map<std::string, LHSType> lhsTypeMap = {
{ "DistanceFromTarget", LHSType::DISTANCE_FROM_TARGET },
{ "StatValue", LHSType::STAT_VALUE },
{ "ItemReady", LHSType::ITEM_READY}
};

const std::unordered_map<std::string, OPType> opTypeMap = {
{ "LessThan", OPType::LT },
{ "GreaterThan", OPType::GT },
{ "Equal", OPType::EQ },
{ "LEQ", OPType::LEQ },
{ "GEQ", OPType::GEQ },
};

class Term
{
public:

	LHSType m_lhsType;
	OPType m_opType;
	std::string m_lhsName;
	float m_rhs;
};


class AIRule
{
public:
	~AIRule();
	std::vector<Term> m_terms;
	size_t m_priority;

	typedef struct 
	{
		bool operator()(const AIRule *a, const AIRule *b)
		{
			return a->m_priority > b->m_priority;
		}

		bool operator()(const AIRule a, const AIRule b)
		{
			return a.m_priority > b.m_priority;
		}
	} CustomCmp;
};

class MovementAIRule : public AIRule
{
public:
	MovementActionType m_action;
};


class ActiveItem;

class ItemAIRule : public AIRule
{
public:
	ActiveItem *m_item;
};
