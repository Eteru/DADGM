#pragma once

#include <vector>
#include "Structs.h"
#include "VisualBody.h"

class Item
{
public:
	Item();
	~Item();

	const StatIncrease & GetStat(StatType type) const;
	const std::vector<StatIncrease> & GetAllStats() const;
	const uint16_t GetOccupiedSpace() const;
	const std::string GetName() const;

	void SetBody(VisualBody *body);

private:
	uint16_t m_occupied_space;
	std::string m_name;
	std::vector<StatIncrease> m_stats;

	VisualBody *m_body;
};

