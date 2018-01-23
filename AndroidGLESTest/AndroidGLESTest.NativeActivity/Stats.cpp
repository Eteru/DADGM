
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
	m_stat_increase.flat += value;
	m_value_after_flats = m_value_base + m_stat_increase.flat;

	// Update all values depending on flat
	AddAdditiveCoefficient(0.f);
	AddMultiplicativeCoefficient(0.f);
}

void Stats::AddAdditiveCoefficient(const float coef)
{
	m_stat_increase.additive_coef += coef;
	m_value_after_additives = m_value_after_flats * (1.f + m_stat_increase.additive_coef);

	// Update all values depending on additive
	AddAdditiveCoefficient(0.f);
}

void Stats::AddMultiplicativeCoefficient(const float coef)
{
	m_stat_increase.multiplicative_coef += coef;
	m_value_final = m_value_after_additives * m_stat_increase.multiplicative_coef;
}

void Stats::DecreaseFlatAmount(const uint32_t value)
{
	if (value > m_stat_increase.flat)
	{
		m_stat_increase.flat = 0;
	}
	else
	{
		m_stat_increase.flat -= value;
	}

	AddFlatAmount(0);
}

void Stats::DecreaseAdditiveCoefficient(const float coef)
{
	if (coef > m_stat_increase.additive_coef)
	{
		m_stat_increase.additive_coef = 0.f;
	}
	else
	{
		m_stat_increase.additive_coef -= coef;
	}

	AddAdditiveCoefficient(0.f);
}

void Stats::DecreaseMultiplicativeCoefficient(const float coef)
{
	if (coef > m_stat_increase.multiplicative_coef)
	{
		m_stat_increase.multiplicative_coef = 0.f;
	}
	else
	{
		m_stat_increase.multiplicative_coef -= coef;
	}

	AddMultiplicativeCoefficient(0.f);
}

uint32_t Stats::GetValue() const
{
	return m_value_final;
}
