#pragma once
#include <vector>
#include <unordered_map>

#include "Structs.h"
#include "GameLoopObject.h"
#include "Math.h"


struct EnumClassHash
{
	template <typename T>
	std::size_t operator()(T t) const
	{
		return static_cast<std::size_t>(t);
	}
};


enum class ModifierType { FLAT = 0, ADDITIVE, MULTIPLICATIVE, NUM_MODIFIER_TYPE };
enum class ModifierTarget { ROBOT = 0, ARMOR, WEAPON, ITEM, NUM_MODIFIER_TARGET };

enum class StatType
{
	HEALTH = 0, MAXIMUM_HEALTH, ARMOR, DAMAGE, LIFE_TIME, BOUNCES, LINEAR_ACCEL, LINEAR_TOP,
	ANGULAR_ACCEL, ANGULAR_TOP, FIRE_RATE, RELOAD_TIME, AMMO, AMMO_MAX, VISION_RANGE, KNOCKBACK, NUM_STAT_TYPE
};

const std::unordered_map<std::string, StatType> xmlTypeMap = {
{"Health", StatType::HEALTH },
{ "MaximumHealth", StatType::MAXIMUM_HEALTH },
{ "Armor", StatType::ARMOR },
{ "Damage", StatType::DAMAGE },
{ "LifeTime", StatType::LIFE_TIME },
{ "Bounces", StatType::BOUNCES },
{ "LinearAccel", StatType::LINEAR_ACCEL },
{ "LinearTop", StatType::LINEAR_TOP },
{ "AngularAccel", StatType::ANGULAR_ACCEL },
{ "AngularTop", StatType::ANGULAR_TOP },
{ "FireRate", StatType::FIRE_RATE },
{ "ReloadTime", StatType::RELOAD_TIME },
{ "Ammo", StatType::AMMO },
{ "MaximumAmmo", StatType::AMMO_MAX },
{ "VisionRange", StatType::VISION_RANGE },
{ "Knockback", StatType::KNOCKBACK }
};

const std::unordered_map<std::string, ModifierType> xmlModTypeMap = {
{ "flat", ModifierType::FLAT },
{ "additive", ModifierType::ADDITIVE },
{ "multiplicative", ModifierType::MULTIPLICATIVE }
};

const std::unordered_map<std::string, ModifierTarget> xmlModTargetMap = {
{ "robot", ModifierTarget::ROBOT },
{ "armor", ModifierTarget::ARMOR },
{ "weapon", ModifierTarget::WEAPON },
{ "item", ModifierTarget::ITEM }
};

class Stat
{
public:
	Stat();
	Stat(const float base);

	float GetValue();
	void AddFlat(const float value);
	void AddAdditive(const float value);
	void AddMultiplicative(const float value);
	void RemoveMultiplicative(const float value);
	void SetBase(const float value);
	void ResetModifiers();
private:

	void ComputeValue();
	bool m_isUpToDate;

	float m_value;
	float m_base;
	float m_flatIncrease;
	float m_additiveCoef;
	std::vector<float> m_multiplicativeCoefs;

};


class StatModifier
{
public:
	StatModifier(const StatType stat, const ModifierType type, const ModifierTarget target, float value) : m_modifiedStat(stat), m_type(type), m_value(value), m_target(target) { m_targetName = ""; }
	StatModifier(const StatType stat, const ModifierType type, const ModifierTarget target, const std::string targetName, float value) : m_modifiedStat(stat), m_type(type), m_value(value), m_target(target), m_targetName(targetName) {}


	ModifierTarget m_target;
	std::string m_targetName;
	StatType m_modifiedStat;
	ModifierType m_type;
	float m_value;
};



class StatHolderObject
{
public:
	void ApplyModifier(const StatModifier &mod);
	void ResetAllModifiers();

	std::unordered_map<StatType, Stat, EnumClassHash> m_stats;
private:
};

// class Stats
// {
// public:
// 	Stats(uint32_t value);
// 	virtual ~Stats();
// 
// 	void AddFlatAmount(const uint32_t value);
// 
// 	/* 0.f = 0%, 1.f = 100% */
// 	void AddAdditiveCoefficient(const float coef);
// 
// 	/* 0.f = 0%, 1.f = 100% */
// 	void AddMultiplicativeCoefficient(const float coef);
// 
// 	void DecreaseFlatAmount(const uint32_t value);
// 
// 	/* 0.f = 0%, 1.f = 100% */
// 	void DecreaseAdditiveCoefficient(const float coef);
// 
// 	/* 0.f = 0%, 1.f = 100% */
// 	void DecreaseMultiplicativeCoefficient(const float coef);
// 
// 	uint32_t GetValue() const;
// 
// private:
// 	uint32_t m_value_base;
// 	uint32_t m_value_after_flats;
// 	uint32_t m_value_after_additives;
// 	uint32_t m_value_final;
// 
// 	StatIncrease m_stat_increase;
// };

