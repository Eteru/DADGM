
#include "Stats.h"

// Stats::Stats(uint32_t value)
// 	: m_value_base(value), m_value_after_flats(value), m_value_after_additives(value),
// 	m_value_final(value)
// {
// }
// 
// Stats::~Stats()
// {
// }
// 
// void Stats::AddFlatAmount(const uint32_t value)
// {
// 	m_stat_increase.flat += value;
// 	m_value_after_flats = m_value_base + m_stat_increase.flat;
// 
// 	// Update all values depending on flat
// 	AddAdditiveCoefficient(0.f);
// 	AddMultiplicativeCoefficient(0.f);
// }
// 
// void Stats::AddAdditiveCoefficient(const float coef)
// {
// 	m_stat_increase.additive_coef += coef;
// 	m_value_after_additives = m_value_after_flats * (1.f + m_stat_increase.additive_coef);
// 
// 	// Update all values depending on additive
// 	AddAdditiveCoefficient(0.f);
// }
// 
// void Stats::AddMultiplicativeCoefficient(const float coef)
// {
// 	m_stat_increase.multiplicative_coef *= coef;
// 	m_value_final = m_value_after_additives * (1.f + m_stat_increase.multiplicative_coef);
// }
// 
// void Stats::DecreaseFlatAmount(const uint32_t value)
// {
// 	if (value > m_stat_increase.flat)
// 	{
// 		m_stat_increase.flat = 0;
// 	}
// 	else
// 	{
// 		m_stat_increase.flat -= value;
// 	}
// 
// 	AddFlatAmount(0);
// }
// 
// void Stats::DecreaseAdditiveCoefficient(const float coef)
// {
// 	if (coef > m_stat_increase.additive_coef)
// 	{
// 		m_stat_increase.additive_coef = 0.f;
// 	}
// 	else
// 	{
// 		m_stat_increase.additive_coef -= coef;
// 	}
// 
// 	AddAdditiveCoefficient(0.f);
// }
// 
// void Stats::DecreaseMultiplicativeCoefficient(const float coef)
// {
// 	if (coef > m_stat_increase.multiplicative_coef)
// 	{
// 		m_stat_increase.multiplicative_coef = 0.f;
// 	}
// 	else
// 	{
// 		m_stat_increase.multiplicative_coef /= coef;
// 	}
// 
// 	AddMultiplicativeCoefficient(0.f);
// }
// 
// uint32_t Stats::GetValue() const
// {
// 	return m_value_final;
// }

Stat::Stat()
{
	m_base = 0;
	m_value = 0;
	m_flatIncrease = 0;
	m_additiveCoef = 0.f;
}

Stat::Stat(const float base)
{
	m_base = base;
	m_value = base;
	m_flatIncrease = 0;
	m_additiveCoef = 0.f;
}

float Stat::GetValue()
{
	if (!m_isUpToDate)
		ComputeValue();

	return m_value;
}

void Stat::AddFlat(const float value)
{
	m_flatIncrease += value;
	m_isUpToDate = false;
}

void Stat::AddAdditive(const float value)
{
	m_additiveCoef += value;
	m_isUpToDate = false;
}

void Stat::AddMultiplicative(const float value)
{
	m_multiplicativeCoefs.push_back(value);
	m_isUpToDate = false;
}

void Stat::RemoveMultiplicative(const float value)
{
	for (auto it = m_multiplicativeCoefs.begin(); it != m_multiplicativeCoefs.end(); ++it)
	{
		if ((*it) == value)
		{
			m_multiplicativeCoefs.erase(it);
			break;
		}
	}

	m_isUpToDate = false;
}

void Stat::SetBase(const float value)
{
	m_base = value;
	m_isUpToDate = false;
}

void Stat::ResetModifiers()
{
	m_flatIncrease = 0.f;
	m_additiveCoef = 0.f;
	m_multiplicativeCoefs.clear();

	m_isUpToDate = false;
}

void Stat::ComputeValue()
{
	m_value = m_base + m_flatIncrease;
	m_value += m_value * m_additiveCoef;

	for (auto multiplicativeCoef : m_multiplicativeCoefs)
	{
		m_value += m_value * multiplicativeCoef;
	}

	m_isUpToDate = true;
}

void StatHolderObject::ApplyModifier(const StatModifier &mod)
{
	if (!m_stats.count(mod.m_modifiedStat))
		return;

	switch (mod.m_type)
	{
	case ModifierType::FLAT:
		m_stats.at(mod.m_modifiedStat).AddFlat(mod.m_value);
		break;
	case ModifierType::ADDITIVE:
		m_stats.at(mod.m_modifiedStat).AddAdditive(mod.m_value);
		break;
	case ModifierType::MULTIPLICATIVE:
		m_stats.at(mod.m_modifiedStat).AddMultiplicative(mod.m_value);
		break;
	default:
		break;
	}
}

void StatHolderObject::RemoveModifier(const StatModifier &mod)
{
	if (!m_stats.count(mod.m_modifiedStat))
		return;

	switch (mod.m_type)
	{
	case ModifierType::FLAT:
		m_stats.at(mod.m_modifiedStat).AddFlat(-mod.m_value);
		break;
	case ModifierType::ADDITIVE:
		m_stats.at(mod.m_modifiedStat).AddAdditive(-mod.m_value);
		break;
	case ModifierType::MULTIPLICATIVE:
		m_stats.at(mod.m_modifiedStat).RemoveMultiplicative(mod.m_value);
		break;
	default:
		break;
	}
}

void StatHolderObject::ResetAllModifiers()
{
	for (auto it = m_stats.begin(); it != m_stats.end(); ++it)
	{
		(*it).second.ResetModifiers();
	}
}