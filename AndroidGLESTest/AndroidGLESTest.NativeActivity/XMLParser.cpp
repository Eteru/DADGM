#include "XMLParser.h"

#include "SceneManager.h"
#include "BresenhamLine.h"
#include "UniqueID.h"

#include "ResourceManager.h"
#include "PhysicsBody.h"
#include "BoundingSphere.h"

#include "ItemDescriptions.h"
// 
// XMLParser::XMLParser(const std::string mapsPath, const std::string itemsPath, const std::string robotsPath)
// {
// 	m_mapsRoot = GetRootOfFile(mapsPath);
// 	m_itemsRoot = GetRootOfFile(itemsPath);
// 	m_robotsRoot = GetRootOfFile(robotsPath);
// 
// 	m_mapNodes = GetAllChildren(m_mapsRoot, "map");
// 	m_armorNodes = GetAllChildren(m_itemsRoot->first_node("armors"), "armor");
// 	m_weaponNodes = GetAllChildren(m_itemsRoot->first_node("weapons"), "weapon");
// 	m_itemNodes = GetAllChildren(m_itemsRoot->first_node("items"), "item");
// 	m_robotNodes = GetAllChildren(m_robotsRoot, "robot");
// }

template<typename T>
void XMLParser::SafeDeleteVec(std::vector<T*> vec)
{
	for (T* elem : vec)
	{
		if (nullptr != elem)
		{
			delete elem;
			elem = nullptr;
		}
	}
}

// XMLParser::~XMLParser()
// {
// // 	SafeDeleteVec(m_mapNodes);
// // 	SafeDeleteVec(m_robotNodes);
// // 	SafeDeleteVec(m_itemNodes);
// // 	SafeDeleteVec(m_armorNodes);
// // 	SafeDeleteVec(m_weaponNodes);
// // 
// // 
// // 	FreeDocString(m_mapsRoot);
// // 	FreeDocString(m_itemsRoot);
// // 	FreeDocString(m_robotsRoot);
// 
// }

MapDescription XMLParser::GetRandomMapDescription()
{
	rapidxml::xml_node<> *mapRoot = GetRootOfFile(configsPath)->first_node("maps");

	auto allMaps = GetAllChildren(mapRoot, "map");

	size_t id = std::rand() % allMaps.size();

	MapDescription result = GetMapDescription(allMaps[id]);

	mapRoot->document()->clear();

	return result;
}

Robot * XMLParser::GetRandomRobot(const Vector2 mapCoords, MapManager *mapManager)
{
	rapidxml::xml_node<> *robotRoot = GetRootOfFile(configsPath)->first_node("robots");
	auto allRobots = GetAllChildren(robotRoot, "robot");

	rapidxml::xml_node<> *itemConfigsRoot = GetRootOfFile(configsPath)->first_node("itemConfigs");
	auto allArmors = GetAllChildren(itemConfigsRoot->first_node("armors"), "armor");
	auto allWeapons = GetAllChildren(itemConfigsRoot->first_node("weapons"), "weapon");
	auto allItems = GetAllChildren(itemConfigsRoot->first_node("items"), "item");

	size_t id = std::rand() % allRobots.size();

	Robot *result = GetRobot(allRobots[id], allArmors, allWeapons, allItems);


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
	pb->m_linkedObject = result;
	pb->AddComponent(result);
	result->m_physicsBody = pb;

	pb->m_acceleration = result->GetArmor()->m_stats.at(StatType::LINEAR_ACCEL).GetValue();
	pb->m_topSpeed = result->GetArmor()->m_stats.at(StatType::LINEAR_TOP).GetValue();
	pb->m_turningAcceleration = result->GetArmor()->m_stats.at(StatType::ANGULAR_ACCEL).GetValue();
	pb->m_topTurningSpeed = result->GetArmor()->m_stats.at(StatType::ANGULAR_TOP).GetValue();


	robotRoot->document()->clear();
	return result;
}

Armor * XMLParser::ParseArmor(rapidxml::xml_node<> *armorNode, std::vector<rapidxml::xml_node<> *> allArmors)
{
	std::string name = XMLUtils::GetStringValueSafe(armorNode, "name", "");


	Armor *armor = new Armor();
	armor->SetID(UniqueID::GetID(armor->GetClassName()));
	armor->Init();

	for (auto armorDef : allArmors)
	{
		std::string defName = XMLUtils::GetStringValueSafe(armorDef, "name", "");

		if (defName == name)
		{

			armor->m_name = defName;
			armor->m_description = XMLUtils::GetStringValueSafe(armorDef, "description", "");

			VisualBody *vb = new VisualBody(Vector3(0.f), Vector3(0.f), Vector3(0.5f), "ArmorVB", true);
			vb->SetID(UniqueID::GetID(vb->GetClassName()));
			vb->SetModel(ResourceManager::GetInstance()->LoadModel(XMLUtils::GetStringValueSafe(armorDef, "model", "2")));
			vb->AddTexture(ResourceManager::GetInstance()->LoadTexture(XMLUtils::GetStringValueSafe(armorDef, "texture", "3")));
			vb->SetShader(ResourceManager::GetInstance()->LoadShader("3"));
			vb->Init();

			armor->AddComponent(vb);

			AddAllStats(armor, armorDef);

			break;
		}
	}

	return armor;

}

Weapon * XMLParser::ParseWeapon(rapidxml::xml_node<> *weaponNode, std::vector<rapidxml::xml_node<> *> allWeapons)
{
	std::string name = XMLUtils::GetStringValueSafe(weaponNode, "name", "");


	Weapon *weapon= new Weapon();
	weapon->SetID(UniqueID::GetID(weapon->GetClassName()));
	weapon->Init();

	for (auto weaponDef : allWeapons)
	{
		std::string defName = XMLUtils::GetStringValueSafe(weaponDef, "name", "");

		if (defName == name)
		{

			weapon->m_name = defName;
			weapon->m_description = XMLUtils::GetStringValueSafe(weaponDef, "description", "");

			VisualBody *vb = new VisualBody(Vector3(0.f, 0.f, -1.f), Vector3(0.f), Vector3(0.25f), "WeaponVB", true);
			vb->SetID(UniqueID::GetID(vb->GetClassName()));
			vb->SetModel(ResourceManager::GetInstance()->LoadModel(XMLUtils::GetStringValueSafe(weaponDef, "model", "2")));
			vb->AddTexture(ResourceManager::GetInstance()->LoadTexture(XMLUtils::GetStringValueSafe(weaponDef, "texture", "3")));
			vb->SetShader(ResourceManager::GetInstance()->LoadShader("3"));
			vb->Init();

			weapon->AddComponent(vb);

			AddAllStats(weapon, weaponDef);

			break;
		}
	}

	return weapon;
}

MapDescription XMLParser::GetMapDescription(rapidxml::xml_node<> *mapNode)
{
	Vector2 dims = XMLUtils::GetVec2ValueXYSafe(mapNode, "dims", Vector2(0, 0));

	std::vector<std::string> mapString(dims.x, std::string(dims.y, '0'));

	bool padding = XMLUtils::GetBoolValueSafe(mapNode, "wallPadding", false);

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

	ParseMapLines(mapNode, dims, mapString);

	MapDescription result;
	result.m_dims = dims;
	result.m_mapString = mapString;

	for (auto spawnPointNode : GetAllChildren(mapNode->first_node("spawnPoints"), "spawnPoint"))
	{
		Vector2 vec = XMLUtils::GetVec2ValueXYSafe(spawnPointNode, Vector2(-1, -1));
		if (vec != Vector2(-1, -1))
		{
			result.m_spawnPoints.push_back(vec);
		}
	}

	return result;
}

Robot * XMLParser::GetRobot(rapidxml::xml_node<> *robotNode, std::vector<rapidxml::xml_node<> *> allArmors, std::vector<rapidxml::xml_node<> *> allWeapons, std::vector<rapidxml::xml_node<> *> allItems)
{
	Robot *result = new Robot();
	result->SetID(UniqueID::GetID(result->GetClassName()));
	result->Init();

	result->m_name = XMLUtils::GetStringValueSafe(robotNode, "name", "");
	result->m_description = XMLUtils::GetStringValueSafe(robotNode, "description", "");

	rapidxml::xml_node<> *armorNode = robotNode->first_node("armor");

	if (nullptr != armorNode)
	{
		Armor *armor = ParseArmor(armorNode, allArmors);
		result->AddComponent(armor);
		result->SetArmor(armor);
	}

	rapidxml::xml_node<> *weaponNode = robotNode->first_node("weapon");

	if (nullptr != weaponNode)
	{
		Weapon *weapon = ParseWeapon(weaponNode, allWeapons);
		result->AddComponent(weapon);
		result->SetWeapon(weapon);
	}

	AddAllStats(result, robotNode);

	return result;
}

void XMLParser::AddAllStats(StatHolderObject *obj, rapidxml::xml_node<> *objectNode)
{
	for (auto statNode : GetAllChildren(objectNode->first_node("stats"), "stat"))
	{
		AddStat(obj, statNode);
	}
}

void XMLParser::AddStat(StatHolderObject *obj, rapidxml::xml_node<> *statNode)
{
	std::string typeString = XMLUtils::GetStringValueSafe(statNode, "name", "");

	if ("" == typeString || 0 == xmlTypeMap.count(typeString))
	{
		return;
	}

	PrintUtils::PrintI(typeString);
	obj->m_stats[xmlTypeMap.at(typeString)] = Stat(XMLUtils::GetFloatValueSafe(statNode, "base", 0));
}

rapidxml::xml_node<> * XMLParser::GetRootOfFile(std::string filePath)
{
	AAssetManager* mgr = SceneManager::GetInstance()->GetEngine()->app->activity->assetManager;
	AAsset* file = AAssetManager_open(mgr, filePath.c_str(), AASSET_MODE_BUFFER);

	if (NULL == file)
		return nullptr;

	long fsize = AAsset_getLength(file);
	char *string = new char[fsize + 1];

	AAsset_read(file, string, fsize);
	AAsset_close(file);

	string[fsize] = '\0';

	rapidxml::xml_document<> doc;
	doc.parse<0>(string);

	rapidxml::xml_node<> *pRoot = doc.first_node();

	//m_nodeMap[pRoot] = string;

	return pRoot;
}

void XMLParser::ParseMapLines(rapidxml::xml_node<> *pNode, Vector2 dims, std::vector<std::string> &mapString)
{
	rapidxml::xml_node<> *pLines = pNode->first_node("lines");
	if (nullptr == pLines)
	{
		return;
	}

	for (rapidxml::xml_node<> *pLine = pLines->first_node("line"); pLine; pLine = pLine->next_sibling())
	{
		ParseMapLine(pLine, dims, mapString);
	}
}

void XMLParser::ParseMapLine(rapidxml::xml_node<> *pNode, Vector2 dims, std::vector<std::string> &mapString)
{
	std::string cellType = XMLUtils::GetStringValueSafe(pNode, "cellType", "");

	if ("wall" == cellType)
	{
		Vector2 from = XMLUtils::GetVec2ValueXYSafe(pNode, "from", Vector2(-1.f));
		Vector2 to = XMLUtils::GetVec2ValueXYSafe(pNode, "to", Vector2(-1.f));

		bool invalidFrom = from.x < 0 || from.x > dims.x - 1 || from.y < 0 || from.y > dims.y - 1;
		bool invalidTo = to.x < 0 || to.x > dims.x - 1 || to.y < 0 || to.y > dims.y - 1;

		if (from.x == -1 || to.x == -1 || invalidFrom || invalidTo)
		{
			return;
		}

		std::vector<Vector2> line = BresenhamLine::Line(from, to);

		for (auto point : line)
		{
			mapString[point.x][point.y] = '1';
		}
	}
}

std::vector<rapidxml::xml_node<> *> XMLParser::GetAllChildren(rapidxml::xml_node<> *node, const std::string name)
{
	std::vector<rapidxml::xml_node<> *> result;
	if (nullptr == node)
	{
		return result;
	}

	for (rapidxml::xml_node<> *pChild = node->first_node(name.c_str()); pChild; pChild = pChild->next_sibling(name.c_str()))
	{
		result.push_back(pChild);
	}

	return result;
}
