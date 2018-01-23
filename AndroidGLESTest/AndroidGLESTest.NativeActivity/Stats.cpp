
#include "Stats.h"

Stats::Stats(uint32_t value)
	: m_value_base(value), m_value_after_flats(value), m_value_after_additives(value),
	m_value_final(value)
{
}

Stats::~Stats()
{
}

void Stats::AddFlatAmount(const uint32_t value)
{
	m_value_after_flats += value;
	m_value_after_additives += value;
	m_value_final += value;
}

void Stats::AddAdditiveCoefficient(const float coef)
{
	int coefficient = static_cast<int>(m_value_base * coef);
	m_value_after_additives += coefficient;
	m_value_final += coefficient;
}

void Stats::AddMultiplicativeCoefficient(const float coef)
{
	int coefficient = static_cast<int>(m_value_base * coef);
	m_value_after_additives *= coefficient;
	m_value_final *= coefficient;
}

uint32_t Stats::GetValue() const
{
	return m_value_final;
}
