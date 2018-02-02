#include "ItemDescriptions.h"

#include "Robot.h"
#include "XMLParser.h"
#include "PhysicsBody.h"
#include "UniqueID.h"
#include "BoundingSphere.h"
#include "ResourceManager.h"
#include "AIController.h"

#include "Stats.h"
#include "VisualBody.h"

ItemDescriptions & ItemDescriptions::GetInstance()
{
	static ItemDescriptions instance;
	return instance;
}

MapDescription ItemDescriptions::GetRandomMap()
{
	return m_maps[std::rand() % m_maps.size()];
}

Robot * ItemDescriptions::GetRandomRobot(const Vector2 mapCoords, MapManager *mapManager)
{
	size_t id = std::rand() % m_robots.size();

	RobotDescription description;

	int cnt = 0;
	for (auto kvPair : m_robots)
	{
		if (cnt++ == id)
		{
			description = kvPair.second;
			break;
		}
	}


	PhysicsBody *pb = SpawnPhysicsBody(mapCoords);
	Robot *robot = SpawnRobot(description);

	robot->m_physicsBody = pb;
	pb->m_linkedObject = robot;

	pb->AddComponent(robot);

	Armor *armor = SpawnArmor(description.m_armor.first);

	for (auto armorModifier : description.m_armor.second)
	{
		armor->ApplyModifier(armorModifier);
	}

	robot->SetArmor(armor);
	robot->AddComponent(armor);

	Weapon *weapon = SpawnWeapon(description.m_weapon.first);

	if (nullptr != weapon)
	{
		for (auto weaponModifier : description.m_weapon.second)
		{
			weapon->ApplyModifier(weaponModifier);
		}

		robot->SetWeapon(weapon);
		robot->AddComponent(weapon);
	}

	for (auto itemPair : description.m_items)
	{
		std::pair<bool, Item *> item = SpawnItem(itemPair.first);
		item.second->m_modifiers.insert(item.second->m_modifiers.end(), itemPair.second.begin(), itemPair.second.end());

		if (item.first)
		{
			robot->m_activeItems.push_back(static_cast<ActiveItem*>(item.second));
		}
		else
		{
			robot->m_passiveItems.push_back(item.second);
		}		
	}

	AIController *ai = new AIController();
	ai->SetID(UniqueID::GetID(ai->GetClassName()));
	ai->m_mapManager = mapManager;
	ai->m_physicsBody = pb;
	ai->m_robot = robot;
	ai->Init();

	MovementAIRule defaultMovement;
	defaultMovement.m_priority = 0;
	defaultMovement.m_action = description.m_AI.m_defaultMovementAction;

	ai->m_movementRules.push_back(defaultMovement);

	ItemAIRule defaultItem;
	defaultItem.m_priority = 0;
	defaultItem.m_item = robot->FindActive(description.m_AI.m_defaultItemAction);

	ai->m_itemRules.push_back(defaultItem);
	

	for (auto movementRule : description.m_AI.m_movementRules)
	{
		MovementAIRule rule;
		rule.m_priority = movementRule.m_priority;
		rule.m_action = movementRule.m_movementAction;
		rule.m_terms = movementRule.m_terms;

		ai->m_movementRules.push_back(rule);
	}

	for (auto itemRule : description.m_AI.m_itemRules)
	{
		ItemAIRule rule;
		rule.m_priority = itemRule.m_priority;
		rule.m_item = robot->FindActive(itemRule.m_itemName);
		rule.m_terms = itemRule.m_terms;

		ai->m_itemRules.push_back(rule);
	}

	robot->AddComponent(ai);
	robot->m_stats[StatType::HEALTH] = robot->m_armor->m_stats.at(StatType::MAXIMUM_HEALTH);
	


	return robot;
}

Robot * ItemDescriptions::SpawnRobot(const RobotDescription &desc) const
{	
	Robot *robot = new Robot();
	robot->SetID(UniqueID::GetID(robot->GetClassName()));

	for (std::pair<std::string, float> kvPair : desc.m_stats)
	{
		robot->m_stats[xmlTypeMap.at(kvPair.first)] = Stat(kvPair.second);
	}

	robot->Init();

	return robot;
}

std::pair<bool, Item *> ItemDescriptions::SpawnItem(const std::string name) const
{
	if (0 == m_basicItems.count(name))
		return std::make_pair(false, nullptr);

	ItemDescription desc = m_basicItems.at(name);

	if (desc.m_type == ItemType::PASSIVE)
	{
		Item *result = new Item();
		result->m_name = desc.m_name;
		result->m_description = desc.m_description;
		
		///TODO Icon

		result->m_modifiers = desc.m_modifiers;
		return std::make_pair(false, result);
	}

	ActiveItem *result = new ActiveItem();
	result->SetID(UniqueID::GetID(result->GetClassName()));
	result->Init();

	Buff buff;
	buff.m_duration = desc.m_buff.m_duration;
	buff.m_modifiers = desc.m_buff.m_modifiers;
	
	result->m_buff = buff;

	return std::make_pair(true, result);;
}

AIController * ItemDescriptions::SpawnAI(const AIDescription &desc) const
{
	
}

Armor * ItemDescriptions::SpawnArmor(const std::string name) const
{

	if (0 == m_armors.count(name))
		return nullptr;

	ArmorDescription desc = m_armors.at(name);

	Armor *result = new Armor();
	result->SetID(UniqueID::GetID(result->GetClassName()));
	result->Init();

	VisualBody *vb = new VisualBody(Vector3(0.f), Vector3(0.f), Vector3(0.5f), "Robot", true);
	//VisualBody *vb = new VisualBody(Vector3(1.f, 0.f, 1.f), Vector3(0.f), Vector3(0.5f), "Robot", true);
	vb->SetID(UniqueID::GetID(vb->GetClassName()));
	vb->SetModel(ResourceManager::GetInstance()->LoadModel(desc.m_model));
	vb->SetShader(ResourceManager::GetInstance()->LoadShader("3"));
	vb->AddTexture(ResourceManager::GetInstance()->LoadTexture(desc.m_texture));

	vb->Init();

	result->AddComponent(vb);

	
	result->m_name = desc.m_name;
	result->m_description = desc.m_description;

	for (std::pair<std::string, float> kvPair : desc.m_stats)
	{
		result->m_stats[xmlTypeMap.at(kvPair.first)] = Stat(kvPair.second);
	}

	return result;
}

Weapon * ItemDescriptions::SpawnWeapon(const std::string name) const
{
	if (0 == m_weapons.count(name))
		return nullptr;

	WeaponDescription desc = m_weapons.at(name);

	Weapon *result = new Weapon();
	result->SetID(UniqueID::GetID(result->GetClassName()));
	result->Init();

	VisualBody *vb = new VisualBody(Vector3(0.f, 0.f, -1.f), Vector3(0.f), Vector3(0.25f), "WeaponVB", true);
	vb->SetID(UniqueID::GetID(vb->GetClassName()));
	vb->SetModel(ResourceManager::GetInstance()->LoadModel(desc.m_model));
	vb->SetShader(ResourceManager::GetInstance()->LoadShader("3"));
	vb->AddTexture(ResourceManager::GetInstance()->LoadTexture(desc.m_texture));

	vb->Init();

	result->AddComponent(vb);


	result->m_name = desc.m_name;
	result->m_description = desc.m_description;

	for (std::pair<std::string, float> kvPair: desc.m_stats)
	{
		result->m_stats[xmlTypeMap.at(kvPair.first)] = Stat(kvPair.second);
	}

	return result;
}

PhysicsBody * ItemDescriptions::SpawnPhysicsBody(const Vector2 mapCoords) const
{
	PhysicsBody *pb = new PhysicsBody();
	pb->SetID(UniqueID::GetID(pb->GetClassName()));
	pb->m_transform.SetPos(GameConstants::ToWorldCoords(mapCoords, GameConstants::WALL_HEIGHT));
	pb->m_transform.ComputeWorld();
	pb->m_transform.m_relative = false;
	pb->m_kinematic = false;
	pb->m_mass = 1.f;
	pb->m_inertia = Math::SphereInertia(pb->m_mass, GameConstants::CELL_SIZE / 2.f);
	pb->m_debugDraw = true;
	pb->Init();

	BoundingSphere *bs = new BoundingSphere();
	bs->SetID(UniqueID::GetID(bs->GetClassName()));
	bs->m_radius = GameConstants::CELL_SIZE / 2.f;
	bs->Init();

	pb->AddComponent(bs);

	return pb;
}

void ItemDescriptions::LoadRobots(std::vector<rapidxml::xml_node<> *> allRobots)
{
	for (auto node : allRobots)
	{
		RobotDescription robot;


		robot.m_name = XMLUtils::GetStringValueSafe(node, "name", "");
		robot.m_description = XMLUtils::GetStringValueSafe(node, "description", "");

	
		std::pair<std::string, std::vector<StatModifier>> armor;

		rapidxml::xml_node<> *armorNode = node->first_node("armor");

		armor.first = XMLUtils::GetStringValueSafe(armorNode, "name", "");

		for (auto modifierNode : XMLParser::GetAllChildren(armorNode->first_node("modifiers"), "modifier"))
		{
			std::string object = XMLUtils::GetStringValueSafe(modifierNode, "object", "");
			std::string target = XMLUtils::GetStringValueSafe(modifierNode, "target", "");
			std::string type = XMLUtils::GetStringValueSafe(modifierNode, "type", "");
			float value = XMLUtils::GetFloatValueSafe(modifierNode, "value", 0);

			StatModifier mod(xmlTypeMap.at(target), xmlModTypeMap.at(type), xmlModTargetMap.at(object), value);

			armor.second.push_back(mod);
		}

		robot.m_armor = armor;


		std::pair<std::string, std::vector<StatModifier>> weapon;

		rapidxml::xml_node<> *weaponNode = node->first_node("weapon");

		weapon.first = XMLUtils::GetStringValueSafe(weaponNode, "name", "");

		for (auto modifierNode : XMLParser::GetAllChildren(weaponNode->first_node("modifiers"), "modifier"))
		{
			std::string object = XMLUtils::GetStringValueSafe(modifierNode, "object", "");
			std::string target = XMLUtils::GetStringValueSafe(modifierNode, "target", "");
			std::string type = XMLUtils::GetStringValueSafe(modifierNode, "type", "");
			float value = XMLUtils::GetFloatValueSafe(modifierNode, "value", 0);

			StatModifier mod(xmlTypeMap.at(target), xmlModTypeMap.at(type), xmlModTargetMap.at(object), value);

			weapon.second.push_back(mod);
		}

		robot.m_weapon = weapon;

	
		rapidxml::xml_node<> *aiNode = node->first_node("AI");

		AIDescription ai;

		ai.m_defaultMovementAction = XMLMovementActionMap.at(XMLUtils::GetStringValueSafe(aiNode, "defaultMovementAction", ""));
		ai.m_defaultItemAction = XMLUtils::GetStringValueSafe(aiNode, "defaultItemAction", "");

		for (auto movementRule : XMLParser::GetAllChildren(aiNode->first_node("movementRules"), "rule"))
		{
			ai.m_movementRules.push_back(ParseMovementRule(movementRule));
		}

		for (auto itemRule : XMLParser::GetAllChildren(aiNode->first_node("itemRules"), "rule"))
		{
			ai.m_itemRules.push_back(ParseActionRule(itemRule));
		}

		robot.m_AI = ai;


		for (auto itemNode : XMLParser::GetAllChildren(aiNode->first_node("items"), "item"))
		{
			std::pair<std::string, std::vector<StatModifier>> item;
			item.first = XMLUtils::GetStringValueSafe(itemNode, "name", "");
			for (auto modifierNode : XMLParser::GetAllChildren(itemNode->first_node("modifiers"), "modifier"))
			{
				std::string object = XMLUtils::GetStringValueSafe(modifierNode, "object", "");
				std::string target = XMLUtils::GetStringValueSafe(modifierNode, "target", "");
				std::string type = XMLUtils::GetStringValueSafe(modifierNode, "type", "");
				float value = XMLUtils::GetFloatValueSafe(modifierNode, "value", 0);

				StatModifier mod(xmlTypeMap.at(target), xmlModTypeMap.at(type), xmlModTargetMap.at(object), value);

				item.second.push_back(mod);
			}

			robot.m_items.push_back(item);

		}

		m_robots[robot.m_name] = robot;
	}
}

void ItemDescriptions::LoadMaps(std::vector<rapidxml::xml_node<> *> allMaps)
{
	for (auto node : allMaps)
	{
		Vector2 dims = XMLUtils::GetVec2ValueXYSafe(node, "dims", Vector2(0, 0));

		std::vector<std::string> mapString(dims.x, std::string(dims.y, '0'));

		bool padding = XMLUtils::GetBoolValueSafe(node, "wallPadding", false);

		if (padding)
		{
			for (int i = 0; i < dims.x; ++i)
			{
				for (int j = 0; j < dims.y; ++j)
				{
					if (i == 0 || j == 0 || i == dims.x - 1 || j == dims.y - 1)
					{
						mapString[i][j] = '1';
					}
				}
			}
		}

		XMLParser::ParseMapLines(node, dims, mapString);

		MapDescription result;
		result.m_dims = dims;
		result.m_mapString = mapString;

		for (auto spawnPointNode : XMLParser::GetAllChildren(node->first_node("spawnPoints"), "spawnPoint"))
		{
			Vector2 vec = XMLUtils::GetVec2ValueXYSafe(spawnPointNode, Vector2(-1, -1));
			if (vec != Vector2(-1, -1))
			{
				result.m_spawnPoints.push_back(vec);
			}
		}

		m_maps.push_back(result);
	}
}

void ItemDescriptions::LoadWeapons(std::vector<rapidxml::xml_node<> *> allWeapons)
{
	for (auto node : allWeapons)
	{
		WeaponDescription weapon;

		weapon.m_name = XMLUtils::GetStringValueSafe(node, "name", "");
		weapon.m_icon = XMLUtils::GetStringValueSafe(node, "icon", "");
		weapon.m_model = XMLUtils::GetStringValueSafe(node, "model", "");
		weapon.m_texture = XMLUtils::GetStringValueSafe(node, "texture", "");
		weapon.m_description = XMLUtils::GetStringValueSafe(node, "description", "");

		for (auto statNode : XMLParser::GetAllChildren(node->first_node("stats"), "stat"))
		{
			std::string typeString = XMLUtils::GetStringValueSafe(statNode, "name", "");

			if ("" == typeString)
			{
				break;
			}

			weapon.m_stats[typeString] = XMLUtils::GetFloatValueSafe(statNode, "base", 0);
			
		}

		m_weapons[weapon.m_name] = weapon;
	}
}

void ItemDescriptions::LoadArmors(std::vector<rapidxml::xml_node<> *> allArmors)
{
	for (auto node : allArmors)
	{
		ArmorDescription armor;

		armor.m_name = XMLUtils::GetStringValueSafe(node, "name", "");
		armor.m_icon = XMLUtils::GetStringValueSafe(node, "icon", "");
		armor.m_model = XMLUtils::GetStringValueSafe(node, "model", "");
		armor.m_texture = XMLUtils::GetStringValueSafe(node, "texture", "");
		armor.m_description = XMLUtils::GetStringValueSafe(node, "description", "");

		for (auto statNode : XMLParser::GetAllChildren(node->first_node("stats"), "stat"))
		{
			std::string typeString = XMLUtils::GetStringValueSafe(statNode, "name", "");

			if ("" == typeString)
			{
				break;
			}

			armor.m_stats[typeString] = XMLUtils::GetFloatValueSafe(statNode, "base", 0);

		}

		m_armors[armor.m_name] = armor;
	}
}

void ItemDescriptions::LoadBasicItems(std::vector<rapidxml::xml_node<> *> allItems)
{
	for (auto node : allItems)
	{
		ItemDescription item;

		item.m_name = XMLUtils::GetStringValueSafe(node, "name", "");
		item.m_icon = XMLUtils::GetStringValueSafe(node, "icon", "");
		item.m_description = XMLUtils::GetStringValueSafe(node, "description", "");
		item.m_type = XMLUtils::GetStringValueSafe(node, "type", "passive") == "passive" ? ItemType::PASSIVE : ItemType::ACTIVE;
		
		if (item.m_type == ItemType::ACTIVE)
		{
			item.m_cooldown = XMLUtils::GetFloatValueSafe(node, "cooldown", 0);			
		}

		for (auto modifierNode : XMLParser::GetAllChildren(node->first_node("modifiers"), "modifier"))
		{
			std::string object = XMLUtils::GetStringValueSafe(modifierNode, "object", "");
			std::string target = XMLUtils::GetStringValueSafe(modifierNode, "target", "");
			std::string type = XMLUtils::GetStringValueSafe(modifierNode, "type", "");
			float value = XMLUtils::GetFloatValueSafe(modifierNode, "value", 0);

			StatModifier mod(xmlTypeMap.at(target), xmlModTypeMap.at(type), xmlModTargetMap.at(object), value);

			item.m_modifiers.push_back(mod);
		}

		rapidxml::xml_node<> *buffNode = node->first_node("buff");

		if (nullptr != buffNode)
		{
			BuffDescription buff;

			buff.m_duration = XMLUtils::GetFloatValueSafe(buffNode, "duration", 0);

			for (auto modifierNode : XMLParser::GetAllChildren(buffNode->first_node("modifiers"), "modifier"))
			{
				std::string object = XMLUtils::GetStringValueSafe(modifierNode, "object", "");
				std::string target = XMLUtils::GetStringValueSafe(modifierNode, "target", "");
				std::string type = XMLUtils::GetStringValueSafe(modifierNode, "type", "");
				std::string targetName = XMLUtils::GetStringValueSafe(modifierNode, "targetName", "");
				float value = XMLUtils::GetFloatValueSafe(modifierNode, "value", 0);

				StatModifier mod(xmlTypeMap.at(target), xmlModTypeMap.at(type), xmlModTargetMap.at(object), targetName, value);

				buff.m_modifiers.push_back(mod);
			}

			item.m_buff = buff;
		}

		m_basicItems[item.m_name] = item;
	}
}

ItemDescriptions::ItemDescriptions()
{
	rapidxml::xml_node<> *docRoot = XMLParser::GetRootOfFile(configsPath);

	LoadMaps(XMLParser::GetAllChildren(docRoot->first_node("maps"), "map"));
	LoadArmors(XMLParser::GetAllChildren(docRoot->first_node("itemConfigs")->first_node("armors"), "armor"));
	LoadWeapons(XMLParser::GetAllChildren(docRoot->first_node("itemConfigs")->first_node("weapons"), "weapon"));
	LoadBasicItems(XMLParser::GetAllChildren(docRoot->first_node("itemConfigs")->first_node("items"), "item"));
	LoadRobots(XMLParser::GetAllChildren(docRoot->first_node("robots"), "robot"));
}

AIMovementRuleDescription ItemDescriptions::ParseMovementRule(rapidxml::xml_node<> * ruleNode)
{
	AIMovementRuleDescription result;

	result.m_priority = XMLUtils::GetIntValueSafe(ruleNode, "priority", 0);
	result.m_movementAction = XMLMovementActionMap.at(XMLUtils::GetStringValueSafe(ruleNode, "action", "StandStill"));


	for (auto termNode : XMLParser::GetAllChildren(ruleNode->first_node("condition"), "term"))
	{
		Term term;
		term.m_lhsType = lhsTypeMap.at(XMLUtils::GetStringValueSafe(termNode, "lhsType", ""));
		term.m_opType = opTypeMap.at(XMLUtils::GetStringValueSafe(termNode, "op", ""));
		term.m_lhsName = XMLUtils::GetStringValueSafe(termNode, "lhs", "");
		term.m_rhs = XMLUtils::GetFloatValueSafe(termNode, "rhs", 0);

		result.m_terms.push_back(term);
	}

	return result;
}

AIItemRuleDescription ItemDescriptions::ParseActionRule(rapidxml::xml_node<> * ruleNode)
{
	AIItemRuleDescription result;

	result.m_priority = XMLUtils::GetIntValueSafe(ruleNode, "priority", 0);
	result.m_itemName = XMLUtils::GetStringValueSafe(ruleNode, "action", "");


	for (auto termNode : XMLParser::GetAllChildren(ruleNode->first_node("condition"), "term"))
	{
		Term term;
		term.m_lhsType = lhsTypeMap.at(XMLUtils::GetStringValueSafe(termNode, "lhsType", ""));
		term.m_opType = opTypeMap.at(XMLUtils::GetStringValueSafe(termNode, "op", ""));
		term.m_lhsName = XMLUtils::GetStringValueSafe(termNode, "lhs", "");
		term.m_rhs = XMLUtils::GetFloatValueSafe(termNode, "rhs", 0);

		result.m_terms.push_back(term);
	}

	return result;
}
