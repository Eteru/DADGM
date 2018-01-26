
#include "Item.h"

Item::Item()
	: m_body(nullptr)
{
	m_stats.resize(StatType::NUM_STATS_TYPES);
}

Item::~Item()
{
}

const StatIncrease & Item::GetStat(StatType type) const
{
	return m_stats[type];
}

const std::vector<StatIncrease>& Item::GetAllStats() const
{
	return m_stats;
}

const uint16_t Item::GetOccupiedSpace() const
{
	return m_occupied_space;
}

const std::string Item::GetName() const
{
	return m_name;
}

void Item::SetBody(VisualBody * body)
{
	m_body = body;
}
