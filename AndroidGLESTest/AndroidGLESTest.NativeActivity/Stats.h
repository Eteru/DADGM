#pragma once

#include "Structs.h"

class Stats
{
public:
	Stats(uint32_t value);
	virtual ~Stats();

	void AddFlatAmount(const uint32_t value);

	/* 0.f = 0%, 1.f = 100% */
	void AddAdditiveCoefficient(const float coef);

	/* 0.f = 0%, 1.f = 100% */
	void AddMultiplicativeCoefficient(const float coef);

	void DecreaseFlatAmount(const uint32_t value);

	/* 0.f = 0%, 1.f = 100% */
	void DecreaseAdditiveCoefficient(const float coef);

	/* 0.f = 0%, 1.f = 100% */
	void DecreaseMultiplicativeCoefficient(const float coef);

	uint32_t GetValue() const;

private:
	uint32_t m_value_base;
	uint32_t m_value_after_flats;
	uint32_t m_value_after_additives;
	uint32_t m_value_final;

	StatIncrease m_stat_increase;
};

