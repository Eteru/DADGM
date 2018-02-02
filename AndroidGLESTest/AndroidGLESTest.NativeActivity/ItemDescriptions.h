#pragma once
#include <unordered_map>
#include <utility>


#include "Item.h"
#include "AIRule.h"

#include "XMLUtils.h"

class MapDescription
{
public:

	std::vector<Vector2> m_spawnPoints;
	Vector2 m_dims;
	std::vector<std::string> m_mapString;
};

class WeaponDescription
{
public:
	std::string m_name;
	std::string m_description;
	std::string m_icon;
	std::string m_model;
	std::string m_texture;

	std::unordered_map<std::string, float> m_stats;
};

typedef WeaponDescription ArmorDescription;

enum class ItemType { PASSIVE, ACTIVE };

class BuffDescription
{
public:
	float m_duration;
	std::vector<StatModifier> m_modifiers;
};

class ItemDescription
{
public:
	std::string m_name;
	std::string m_description;
	std::string m_icon;

	ItemType m_type;
	float m_cooldown;

	BuffDescription m_buff;
	std::vector<StatModifier> m_modifiers;
};

class AIItemRuleDescription
{
public:
	std::vector<Term> m_terms;
	std::string m_itemName;
	size_t m_priority;
};

class AIMovementRuleDescription
{
public:
	std::vector<Term> m_terms;
	MovementActionType m_movementAction;
	size_t m_priority;
};

class AIDescription
{
public:
	std::vector<AIItemRuleDescription> m_itemRules;
	std::vector<AIMovementRuleDescription> m_movementRules;

	MovementActionType m_defaultMovementAction;
	std::string m_defaultItemAction;
};

class RobotDescription
{
public:
	std::string m_name;
	std::string m_description;

	std::pair<std::string, std::vector<StatModifier>> m_armor;
	std::pair<std::string, std::vector<StatModifier>> m_weapon;

	AIDescription m_AI;

	std::vector<std::pair<std::string, std::vector<StatModifier>>> m_items;
	std::unordered_map<std::string, float> m_stats;
};

class Robot;
class AIController;
class Weapon;
class Armor;
class PhysicsBody;

class ItemDescriptions
{
public:

	static ItemDescriptions & GetInstance();
	MapDescription GetRandomMap();
	Robot *GetRandomRobot(const Vector2 mapCoords, MapManager *mapManager);

private:

	Robot * SpawnRobot(const RobotDescription &desc) const;
	std::pair<bool, Item *> SpawnItem(const std::string name) const;
	AIController * SpawnAI(const AIDescription &desc) const;
	Armor * SpawnArmor(const std::string name) const;
	Weapon * SpawnWeapon(const std::string name) const;
	PhysicsBody *SpawnPhysicsBody(const Vector2 mapCoords) const;

	void LoadRobots(std::vector<rapidxml::xml_node<> *> allRobots);
	void LoadMaps(std::vector<rapidxml::xml_node<> *> allMaps);
	void LoadWeapons(std::vector<rapidxml::xml_node<> *> allWeapons);
	void LoadArmors(std::vector<rapidxml::xml_node<> *> allArmors);
	void LoadBasicItems(std::vector<rapidxml::xml_node<> *> allItems);

	ItemDescriptions();


	AIMovementRuleDescription ParseMovementRule(rapidxml::xml_node<> * ruleNode);
	AIItemRuleDescription ParseActionRule(rapidxml::xml_node<> * ruleNode);

	std::vector<MapDescription> m_maps;
	std::unordered_map<std::string, RobotDescription> m_robots;
	std::unordered_map<std::string, WeaponDescription> m_weapons;
	std::unordered_map<std::string, ArmorDescription> m_armors;
	std::unordered_map<std::string, ItemDescription> m_basicItems;

};
